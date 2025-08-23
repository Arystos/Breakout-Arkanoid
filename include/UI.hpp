// UI.hpp
#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

namespace UI {

    // Text alignment
    enum class AlignH { Left, Center, Right };

    // Label 
    struct Label {
        std::string text;          // testo UTF-8
        SDL_Texture* tex = nullptr;// texture generata da TTF
        SDL_Rect dst{0,0,0,0};     // posizione e dimensioni sullo schermo
        SDL_Color color{255,255,255,255};
        TTF_Font* font = nullptr;
        AlignH align = AlignH::Left;
    };

    // Stato visuale del pulsante
    enum class ButtonVisual { Normal, Hover, Pressed, Disabled };

    // Pulsante semplice con sfondo pieno e bordo
    struct Button {
        Label caption;             // etichetta del pulsante
        SDL_Rect rect{0,0,0,0};    // frame del pulsante (incluso padding)
        int paddingX = 16;         // padding orizzontale per il testo
        int paddingY = 8;          // padding verticale per il testo
        SDL_Color bgNormal{30,30,30,255};
        SDL_Color bgHover{50,50,50,255};
        SDL_Color bgPressed{70,70,70,255};
        SDL_Color bgDisabled{40,40,40,255};
        SDL_Color border{200,200,200,255};
        int borderWidth = 2;

        bool enabled = true;
        bool hovered = false;
        bool pressed = false;
    };

    // Crea una texture da testo UTF-8. Restituisce nullptr in caso di errore.
    SDL_Texture* MakeText(SDL_Renderer* r, TTF_Font* f, const std::string& s, SDL_Color col);

    // Riempi Label con nuova texture da testo e aggiorna dst.w/h. Non libera la precedente.
    bool BuildLabel(SDL_Renderer* r, Label& L, const std::string& text, TTF_Font* font, SDL_Color color, AlignH align = AlignH::Left);

    // Aggiorna l'allineamento del testo all'interno del rettangolo target (solo X).
    void AlignLabelX(Label& L, const SDL_Rect& container, AlignH align);

    // Disegna una Label. Non fa clipping.
    void DrawLabel(SDL_Renderer* r, const Label& L);

    // Distrugge in sicurezza la texture della Label
    void Destroy(Label& L);

    // Inizializza il pulsante con testo e calcola rect in base a padding.
    // centerX se true centra il pulsante orizzontalmente rispetto a containerW.
    bool BuildButton(SDL_Renderer* r, Button& B, const std::string& text, TTF_Font* font,
                     SDL_Color textColor,
                     int x, int y, int containerW = 0, bool centerX = false);

    // Imposta posizione del pulsante. Se centerX è true centra orizzontalmente in containerW.
    void SetButtonPosition(Button& B, int x, int y, int containerW = 0, bool centerX = false);

    // Gestione eventi mouse del pulsante. Ritorna true se è stato "cliccato" (press+release dentro).
    bool HandleButtonEvent(Button& B, const SDL_Event& e);

    // Disegna il pulsante con stato visivo coerente (Normal/Hover/Pressed/Disabled).
    void DrawButton(SDL_Renderer* r, const Button& B);

    // Distrugge risorse del pulsante (solo texture del caption).
    void Destroy(Button& B);

    // Helpers di layout:

    // Dispone verticalmente una lista di SDL_Rect* con spaziatura costante. Opzionale centratura orizzontale.
    // startY: top iniziale, spacing: spazio tra elementi, containerW: larghezza per centratura.
    void LayoutVertical(std::vector<SDL_Rect*>& rects, int startY, int spacing, int containerW = 0, bool centerX = false);

    // Centra un SDL_Rect orizzontalmente rispetto a containerW (aggiorna r.x).
    inline void CenterHoriz(SDL_Rect& r, int containerW) { r.x = (containerW - r.w) / 2; }

    // Utility: controlla se un punto è dentro un rettangolo.
    inline bool PointInRect(int mx, int my, const SDL_Rect& r) {
        return mx >= r.x && mx < (r.x + r.w) && my >= r.y && my < (r.y + r.h);
    }

} // namespace UI