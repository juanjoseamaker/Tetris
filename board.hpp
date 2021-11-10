#pragma once

#include <SDL2/SDL.h>

const int g_dim = 10;

class Board {
private:
    char m_board[g_dim][g_dim] = {0};
    int m_tileSize;
    int m_currentPiece = 2, m_currentPieceX = g_dim/2, m_currentPieceY = 1;
    int m_currentRotation = 0;
    int m_dtCurrentRotation = 0;
    int m_dtCurrentPieceX = 0;

    void putCurrentPiece();
    void clearCurrentPiece();
    bool isCurrentPieceColliding();
    
public:
    Board(int tileSize);
    void update();
    void draw(SDL_Renderer *renderer);
    void changeCurrentRotation(int rotation);
    void changeCurrentPieceX(int change);
};