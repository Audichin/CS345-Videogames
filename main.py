#!/usr/bin/env python3
import itertools
import json
import math
import socket
import time
import urllib.error
import urllib.request
import urllib.parse

class Phyphox:
    def __init__(self, url, timeout=1.000, socket_timeout_retries=4):
        u = urllib.parse.urlparse(url)
        if not u.netloc:
            u = urllib.parse.urlparse("//" + url)
        if not u.scheme:
            u = u._replace(scheme="http")
        u = u._replace(path="", params="", query="", fragment="")
        self.__addr = u.geturl()

        self.timeout = timeout
        self.socket_timeout_retries = socket_timeout_retries

    def url(self):
        """
        Gets the current base URL.
        """
        return self.__addr

    def json(self, path):
        """
        Makes a JSON request to Phyphox.
        """
        tries = 0
        while True:
            try:
                tries += 1
                # note: must use urllib (or something else which doesn't keep-alive,
                # since that will break Phyphox)
                with urllib.request.urlopen(self.__addr + path, timeout=self.timeout) as r:
                    if r.status != 200:
                        raise Exception("invalid phyphox response: status {r.status}")
                    return json.loads(r.read())
            except urllib.error.URLError as err:
                if isinstance(err.reason, socket.timeout):
                    if tries < self.socket_timeout_retries:
                        continue
                raise err
            except socket.timeout as err:
                if tries < self.socket_timeout_retries:
                    continue
                raise err

    def control(self, cmd):
        """
        Sends a command to Phyphox.
        """
        obj = self.json(f"/control?cmd={cmd}")
        if "result" not in obj:
            raise Exception("invalid phyphox response: does not contain result")
        if not obj["result"]:
            raise Exception(f"phyphox error: failed to execute {cmd}")

    def get(self, time_var, since=None, *vars):
        """
        Get the experiment data, and whether the experiment is running, for the
        dependent vars, using the independent variable time_var. If since is an
        integer greater than or equal to 0, everything since then is returned.
        Otherwise, only the latest sample is returned.
        """
        vars = [time_var, *vars]

        # build the request params
        prt = since is not None and since >= 0
        qry = "&".join(
            [f"{time_var}={since:.4f}"       if prt else time_var] +
            [f"{var}={since:.4f}|{time_var}" if prt else var for var in vars])
        obj = self.json(f"/get?{qry}")

        # expected update modes
        upd = {"full", "partial"} if prt else {"single"}

        # validate
        if "status" not in obj or type(obj["status"]) != dict:
            raise Exception(f"invalid phyphox response: missing status object")
        if "measuring" not in obj["status"] or type(obj["status"]["measuring"]) != bool:
            raise Exception(f"invalid phyphox response: missing status.measuring bool")
        if "buffer" not in obj or type(obj["buffer"]) != dict:
            raise Exception(f"invalid phyphox response: missing buffer object")

        # status
        mea = obj["status"]["measuring"]

        # load the data
        dat = dict()
        for var in vars:
            # check if the var actually exists
            if var not in obj["buffer"]:
                raise Exception(f"phyphox error: variable {var} does not exist for the current experiment")

            # validate
            if type(obj["buffer"][var]) != dict:
                raise Exception(f"invalid phyphox response: buffer.{var} is not an object")
            if "updateMode" not in obj["buffer"][var] or type(obj["buffer"][var]["updateMode"]) != str:
                raise Exception(f"invalid phyphox response: missing buffer.{var}.updateMode string")
            if "buffer" not in obj["buffer"][var] or type(obj["buffer"][var]["buffer"]) != list:
                raise Exception(f"invalid phyphox response: buffer.{var}.buffer is not an array")

            # ensure the update mode is what we think it should be
            if obj["buffer"][var]["updateMode"] not in upd:
                raise Exception(f"invalid phyphox response: unexpected updateMode for {var}: got {obj['buffer'][var]['updateMode']}, expected one of {', '.join(upd)} (this is likely a bug in our code)")

            # get the values
            buf = obj["buffer"][var]["buffer"]

            # if length is zero (for partial updates), or the first item is
            # null (for single updates), we haven't recorded any data yet
            if len(buf) == 0 or (len(buf) == 1 and buf[0] is None):
                buf = []


            # check the data for consistency
            if len(buf):
                if prt:
                    if var == time_var:
                        # round the timestamp to avoid error
                        buf = [round(x, 5) for x in buf]

                        # must be greater than the start timestamp
                        if buf[0] < since:
                            raise Exception(f"invalid phyphox response: first timestamp {buf[0]:.4f} is older than expected start {since}")

                        # must be monotonous without duplicates
                        for prev, cur in zip(buf, buf[1:]):
                            if cur == prev:
                                raise Exception(f"invalid phyphox response: duplicate timestamp in data ({cur:.4f})")
                            if cur < prev:
                                raise Exception(f"invalid phyphox response: time does not move forwards ({prev:.4f}, {cur:.4f})")
                    else:
                        # check if we have extra samples at the end
                        # - this should never happen on Android
                        # - on iOS, it seems to write the internal buffer directly
                        #   to the response without copying/synchronization, which
                        #   means it sometimes returns a few extra samples
                        n_exp, n_act = len(dat[time_var]), len(buf)
                        if max(n_exp, n_act) - min(n_exp, n_act) > 15:
                            raise Exception(f"invalid phyphox response: expected ~{n_exp} values for {var}, got {n_act} (this is likely a bug in phyphox or our code)")
                else:
                    # a single update should have at most one value
                    if len(buf) > 1:
                        raise Exception(f"invalid phyphox response: more than one value ({len(buf)}) returned for single update")

            # done
            dat[var] = buf

        # put the columns together, dropping extra samples at the end of columns
        # if needed
        dat = list(zip(*[dat[var] for var in vars]))

        # done
        return dat, mea

    def is_measuring(self):
        """
        Checks if the experiment is currently running.
        """
        # On Android /get works, and with a non-existent column (/get?dummy), it
        # returns the status and an empty buffer. On iOS /get doesn't work
        # (401, it wants columns), and with a non-existent column (/get?dummy), it
        # returns the status and an empty buffer.
        obj = self.json("/get?dummy")
        if "status" not in obj or type(obj["status"]) != dict:
            raise Exception(f"invalid phyphox response: missing status object")
        if "measuring" not in obj["status"] or type(obj["status"]["measuring"]) != bool:
            raise Exception(f"invalid phyphox response: missing status.measuring bool")
        return obj["status"]["measuring"]

    def wait_measuring(self, measuring=True, tries=4, interval=0.25):
        """
        Polls the state of the experiment up to {tries} every {interval},
        returning True when the state matches, and False otherwise.
        """
        for i in range(tries):
            if self.is_measuring() == measuring:
                return True
            time.sleep(interval)
        return False

    def clear(self, no_sleep=False):
        """
        Clears the experiment buffer and stops recording data. Not guaranteed to
        take effect immediately.
        """
        self.control("clear")
        if not no_sleep:
            time.sleep(0.25)

    def stop(self, no_sleep=False):
        """
        Stops recording data. Not guaranteed to take effect immediately; use
        wait_measuring (and optionally no_sleep) to check for that.
        """
        self.control("stop")
        if not no_sleep:
            time.sleep(0.25)

    def start(self, no_sleep=False):
        """
        Starts recording data. Not guaranteed to take effect immediately; use
        wait_measuring (and optionally no_sleep) to check for that.
        """
        self.control("start")
        if not no_sleep:
            time.sleep(0.5)

def ticker(interval, resolution=0.01):
    """
    Returns a generator which yields a value, evenly spaced at interval, queuing
    up to one missed tick.
    """
    ts = time.monotonic()
    if interval <= 0:
        raise ValueError("window must be positive")
    while True:
        tc = ts
        while True:
            cur = time.monotonic()
            if ts <= cur:
                while ts <= cur:
                    ts += interval
                break
            if ts - cur - resolution*2 > 0:
                time.sleep(ts - cur - resolution*2)
            else:
                time.sleep(resolution)
        yield tc

if __name__ == "__main__":
    import argparse
    import sys
    import signal

    parser = argparse.ArgumentParser("phyphox_live")
    parser.add_argument("-i", "--interval", help="Best-effort interval to get the next set of samples at (seconds).", type=float, default=0.500)
    parser.add_argument("-t", "--timeout", help="HTTP timeout (seconds).", type=float, default=1.000)
    parser.add_argument("-v", "--verbose", help="Show additional debugging information.", action=argparse.BooleanOptionalAction)
    parser.add_argument("host", help="Phyphox HTTP host.", type=str)
    args = parser.parse_args()

    pp = Phyphox(args.host, timeout=args.timeout)

    print("clearing data", file=sys.stderr)
    pp.clear()
    if not pp.wait_measuring(False):
        raise Exception("phyphox didn't clear the buffer")

    print("starting recording", file=sys.stderr)
    pp.start()
    if not pp.wait_measuring(True):
        raise Exception("phyphox didn't start recording")

    def interrupt(*a):
        print("stopping recording", file=sys.stderr)
        pp.stop(no_sleep=True)
        exit(0)
    signal.signal(signal.SIGINT, interrupt)

    print("getting data", file=sys.stderr)
    t = 0
    print("acc_time,accX,accY,accZ")
    for _ in ticker(args.interval):
        buf, mea = pp.get("acc_time", t, "accX", "accY", "accZ")
        if not mea or round(t, 3) > round(buf[-1][0], 3):
            print("recording stopped", file=sys.stderr)
            exit(1)

        t = buf[-1][0]
        for acc_time, accX, accY, accZ in buf:
            print(f"{acc_time:.4f},{accX:.2f},{accY:.2f},{accZ:.2f}")