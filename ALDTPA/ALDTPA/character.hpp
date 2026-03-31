#pragma once

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <SDL.h>

#include "mediamanager.hpp"

class Sprite
{
protected:
    std::vector<SDL_Texture *> textures;
    SDL_Rect src, dst;
    int which;
    bool animated, dead;
    int totalMillis;
    void moveTo(int newX, int newY)
    {
        dst.x = newX;
        dst.y = newY;
    }

public:
    SDL_Rect getDestination() const { return dst; }
    int getX() const { return dst.x; }
    int getY() const { return dst.y; }
    int getWidth() const { return dst.w; }
    int getHeight() const { return dst.h; }
    void setSize(int newWidth, int newHeight)
    {
        dst.w = newWidth;
        dst.h = newHeight;
    }
    void scaleToFit(int maxWidth, int maxHeight)
    {
        if (src.w <= 0 || src.h <= 0)
            return;

        const float widthScale = static_cast<float>(maxWidth) / static_cast<float>(src.w);
        const float heightScale = static_cast<float>(maxHeight) / static_cast<float>(src.h);
        const float scale = std::min(1.0f, std::min(widthScale, heightScale));

        dst.w = std::max(1, static_cast<int>(src.w * scale));
        dst.h = std::max(1, static_cast<int>(src.h * scale));
    }
    void setPosition(int newX, int newY)
    {
        moveTo(newX, newY);
    }
    void translate(int deltaX, int deltaY)
    {
        moveTo(dst.x + deltaX, dst.y + deltaY);
    }
    void stop() { animated = false; }
    void go() { animated = true; }
    void die() { dead = true; }
    bool inside(int x, int y) const
    {
        return (dst.x <= x && x <= dst.x + dst.w && dst.y <= y && y <= dst.y + dst.h);
    }
    bool collided(Sprite *other) const
    {
        return inside(other->dst.x, other->dst.y) ||
               inside(other->dst.x + other->dst.w, other->dst.y) ||
               inside(other->dst.x, other->dst.y + other->dst.h) ||
               inside(other->dst.x + other->dst.w, other->dst.y + other->dst.h);
    }
    virtual void update(float dt)
    {
        if (!animated || dead)
            return;
        totalMillis += static_cast<int>(dt * 1000.0f);
        int frameTime = 250;
        while (totalMillis > frameTime)
        {
            which++;
            if (which >= static_cast<int>(textures.size()))
                which = 0;
            totalMillis -= frameTime;
        }
    }
    void draw(SDL_Renderer *renderer, int offsetx = 0, int offsety = 0)
    {
        if (dead)
            return;
        if (textures.empty() || textures[which] == NULL)
            throw "About to render a null texture";
        SDL_Rect finalDst = dst;
        finalDst.x += offsetx;
        finalDst.y += offsety;
        SDL_RenderCopy(renderer, textures[which], &src, &finalDst);
    }
    Sprite(SDL_Renderer *renderer, int count = 1, const std::string &fname = "image", const std::string &exten = ".bmp",
           int newX = 0, int newY = 0)
        : src{0, 0, 0, 0}, dst{newX, newY, 0, 0}, which(0), animated(true), dead(false), totalMillis(0)
    {
        for (int i = 0; i < count; i++)
        {
            std::stringstream ss;
            if (count == 1)
            {
                ss << fname << exten;
            }
            else
            {
                ss << fname << i << exten;
            }
            textures.push_back(mm.read(renderer, ss.str(), src.w, src.h));
        }
        dst.w = src.w;
        dst.h = src.h;
    }
};

class Character : public Sprite
{
    float px, py, vx, vy, ax, ay;

    public:
        Character(SDL_Renderer *renderer, int count = 1, const std::string &fname = "image", const std::string &exten = ".bmp",
                int newX = 0, int newY = 0,
                float newVx = 0.0f, float newVy = 0.0f,
                float newAx = 0.0f, float newAy = 0.0f)
            : Sprite(renderer, count, fname, exten, newX, newY),
            px(static_cast<float>(newX)),
            py(static_cast<float>(newY)),
            vx(newVx),
            vy(newVy),
            ax(newAx),
            ay(newAy)
    {
    }

    void update(float dt) override
    {
        if (dead)
            return;
        Sprite::update(dt);
        vx = vx + ax * dt;
        px = px + vx * dt;
        vy = vy + ay * dt;
        py = py + vy * dt;
        moveTo(static_cast<int>(px), static_cast<int>(py));
    }

    void setPosition(int newX, int newY)
    {
        px = static_cast<float>(newX);
        py = static_cast<float>(newY);
        Sprite::setPosition(newX, newY);
    }

    void translate(int deltaX, int deltaY)
    {
        setPosition(static_cast<int>(px) + deltaX, static_cast<int>(py) + deltaY);
    }

    void keyEvent(SDL_Keycode symbol)
    {
        if (symbol == SDLK_SPACE)
        {
            vx = vx + 10.0f;
            vy = vy - 10.0f;
        }
        if (symbol == SDLK_a)
            px -= 32.0f;
        if (symbol == SDLK_d)
            px += 32.0f;
        if (symbol == SDLK_w)
            py -= 32.0f;
        if (symbol == SDLK_s)
            py += 32.0f;
        moveTo(static_cast<int>(px), static_cast<int>(py));
    }
};
