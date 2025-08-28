// UI.cpp
#include "UI.hpp"
#include <algorithm>
#include <iostream>

namespace UI {

    // --- interni ---

    // Imposta colore e disegna un rettangolo pieno
    static inline void fillRect(SDL_Renderer* r, const SDL_Rect& rc, SDL_Color c) {
        SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(r, &rc);
    }

    // Disegna bordo rettangolo con spessore
    static inline void drawRectBorder(SDL_Renderer* r, const SDL_Rect& rc, SDL_Color c, int w) {
        SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
        for (int i=0; i<w; ++i) {
            SDL_Rect o{ rc.x - i, rc.y - i, rc.w + i*2, rc.h + i*2 };
            SDL_RenderDrawRect(r, &o);
        }
    }

    // Calcola dimensioni texture
    static inline void querySize(SDL_Texture* t, int& w, int& h) {
        w = h = 0; if (t) SDL_QueryTexture(t, nullptr, nullptr, &w, &h);
    }

    // --- API ---

    SDL_Texture* MakeText(SDL_Renderer* r, TTF_Font* f, const std::string& s, SDL_Color col) {
        if (!r || !f) return nullptr;
        if (s.empty()) return nullptr;
        SDL_Surface* surf = TTF_RenderUTF8_Blended(f, s.c_str(), col);
        if (!surf) { SDL_Log("TTF_RenderUTF8_Blended: %s", TTF_GetError()); return nullptr; }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
        SDL_FreeSurface(surf);
        if (!tex) SDL_Log("SDL_CreateTextureFromSurface: %s", SDL_GetError());
        return tex;
    }

    bool BuildLabel(SDL_Renderer* r, Label& L, const std::string& text, TTF_Font* font, SDL_Color color, AlignH align) {
        if (!r || !font) return false;
        // Se nulla è cambiato, riusa la texture
        if (L.tex && L.font == font && L.text == text
            && L.color.r == color.r && L.color.g == color.g
            && L.color.b == color.b && L.color.a == color.a) {
            L.align = align;
            if (L.container.w == 0 && L.container.h == 0) L.container = L.dst;
            return true;
        }

        if (L.tex) { SDL_DestroyTexture(L.tex); L.tex = nullptr; }

        L.text  = text;
        L.font  = font;
        L.color = color;
        L.align = align;

        L.tex = MakeText(r, font, text, color);
        if (!L.tex) return false;

        int w = 0, h = 0;
        querySize(L.tex, w, h);
        L.dst.w = w; L.dst.h = h;              // nessuno scaling via dst
        if (L.container.w == 0 && L.container.h == 0)
            L.container = L.dst;               // default al proprio rect

        return true;
    }

    // Aggiorna solo il testo/colore. Non ricrea se invariato.
    bool SetLabelText(SDL_Renderer* r, Label& L, const std::string& text, SDL_Color color) {
        if (!r || !L.font) return false;

        if (L.tex && L.text == text
            && L.color.r == color.r && L.color.g == color.g
            && L.color.b == color.b && L.color.a == color.a) {
            return true;
        }

        if (L.tex) { SDL_DestroyTexture(L.tex); L.tex = nullptr; }

        L.text = text;
        L.color = color;

        L.tex = MakeText(r, L.font, text, color);
        if (!L.tex) return false;

        int w = 0, h = 0;
        querySize(L.tex, w, h);
        L.dst.w = int(w * L.scale);
        L.dst.h = int(h * L.scale);
        if (L.container.w == 0 && L.container.h == 0)
            L.container = L.dst;

        return true;
    }

    void AlignLabelX(Label& L, const SDL_Rect& container, AlignH align) {
        L.align = align;
        switch (align) {
            case AlignH::Left:   L.dst.x = container.x; break;
            case AlignH::Center: L.dst.x = container.x + (container.w - L.dst.w) / 2; break;
            case AlignH::Right:  L.dst.x = container.x + container.w - L.dst.w; break;
        }
    }

    void DrawLabel(SDL_Renderer* r, const Label& L) {
        if (!r || !L.tex) return;
        SDL_RenderCopy(r, L.tex, nullptr, &L.dst);
    }

    void Destroy(Label& L) {
        if (L.tex) { SDL_DestroyTexture(L.tex); L.tex = nullptr; }
        // Font non viene chiuso qui (non posseduto).
    }

    static void layoutCaptionInButton(Button& B) {
        // Centra il testo nel rettangolo del pulsante in base al padding
        B.caption.dst.x = B.rect.x + (B.rect.w - B.caption.dst.w) / 2;
        B.caption.dst.y = B.rect.y + (B.rect.h - B.caption.dst.h) / 2;
    }

    bool BuildButton(SDL_Renderer* r, Button& B, const std::string& text, TTF_Font* font,
                     SDL_Color textColor,
                     int x, int y, int containerW, bool centerX) {
        if (!BuildLabel(r, B.caption, text, font, textColor, AlignH::Center)) return false;

        // Calcola dimensioni del pulsante in base al testo + padding
        B.rect.w = B.caption.dst.w + B.paddingX * 2;
        B.rect.h = B.caption.dst.h + B.paddingY * 2;

        // Posizionamento
        B.rect.y = y;
        if (centerX && containerW > 0) {
            CenterHoriz(B.rect, containerW);
        } else {
            B.rect.x = x;
        }

        // Centra il caption all'interno del pulsante
        layoutCaptionInButton(B);
        return true;
    }

    void SetButtonPosition(Button& B, int x, int y, int containerW, bool centerX) {
        B.rect.y = y;
        if (centerX && containerW > 0) {
            CenterHoriz(B.rect, containerW);
        } else {
            B.rect.x = x;
        }
        layoutCaptionInButton(B);
    }

    bool HandleButtonEvent(Button& B, const SDL_Event& e) {
        if (!B.enabled) return false;

        int mx=0, my=0;
        bool inside = false;

        // Aggiorna hover con movimento mouse
        if (e.type == SDL_MOUSEMOTION) {
            mx = e.motion.x; my = e.motion.y;
            inside = PointInRect(mx, my, B.rect);
            B.hovered = inside;
            return false;
        }

        // Press
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            mx = e.button.x; my = e.button.y;
            inside = PointInRect(mx, my, B.rect);
            B.pressed = inside; // pressed solo se partito dentro
            return false;
        }

        // Release: click valido se era pressed e il rilascio avviene dentro
        if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
            mx = e.button.x; my = e.button.y;
            inside = PointInRect(mx, my, B.rect);
            bool clicked = B.pressed && inside;
            B.pressed = false;
            B.hovered = inside;
            return clicked;
        }

        return false;
    }

    void DrawButton(SDL_Renderer* r, const Button& B) {
        if (!r) return;

        // Colore di sfondo in base allo stato
        ButtonVisual vis = ButtonVisual::Normal;
        if (!B.enabled)           vis = ButtonVisual::Disabled;
        else if (B.pressed)       vis = ButtonVisual::Pressed;
        else if (B.hovered)       vis = ButtonVisual::Hover;

        SDL_Color bg = B.bgNormal;
        if (vis == ButtonVisual::Hover)   bg = B.bgHover;
        if (vis == ButtonVisual::Pressed) bg = B.bgPressed;
        if (vis == ButtonVisual::Disabled)bg = B.bgDisabled;

        // Sfondo
        fillRect(r, B.rect, bg);

        // Bordo
        if (B.borderWidth > 0) drawRectBorder(r, B.rect, B.border, B.borderWidth);

        // Testo
        DrawLabel(r, B.caption);
    }

    void Destroy(Button& B) {
        Destroy(B.caption);
    }

    void LayoutVertical(std::vector<SDL_Rect*>& rects, int startY, int spacing, int containerW, bool centerX) {
        int y = startY;
        for (auto* r : rects) {
            if (!r) continue;
            r->y = y;
            if (centerX && containerW > 0) CenterHoriz(*r, containerW);
            y += r->h + spacing;
        }
    }

    void VBox_PerformLayout(VerticalBox& box) {
        if (box.items.empty()) return;
        int y = box.rect.y + box.padding;
        for (auto* b : box.items) {
            if (!b) continue;
            b->rect.y = y;
            if (box.centerX && box.rect.w > 0) {
                CenterHoriz(b->rect, box.rect.w);
                layoutCaptionInButton(*b);
            }
            if (box.expandWidth && box.rect.w > 0) {
                b->rect.w = box.rect.w - box.padding * 2;
                if (box.equalWidth) {
                    int maxW = 0;
                    for (auto *bi: box.items) { if (bi && bi->rect.w > maxW) maxW = bi->rect.w; }
                    b->rect.w = maxW;
                }
            }
            layoutCaptionInButton(*b);
            y += b->rect.h + box.spacing;
        }
    }


    void HBox_PerformLayout(HorizontalBox& box) {
        if (box.items.empty()) return;
        int x = box.rect.x + box.padding;
        for (auto* b : box.items) {
            if (!b) continue;
            b->rect.x = x;
            if (box.centerY && box.rect.h > 0) {
                CenterVert(b->rect, box.rect.h);
                layoutCaptionInButton(*b);
            }
            if (box.expandHeight && box.rect.h > 0) {
                b->rect.h = box.rect.h - box.padding * 2;
            }
            layoutCaptionInButton(*b);
            x += b->rect.w + box.spacing;
        }
    }

}