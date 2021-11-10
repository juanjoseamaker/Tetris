#include "board.hpp"

#include <SDL2/SDL.h>

#include <iostream>

const int g_ticksForMove = 5;
int ticks = 0;

const char** pieces[] = {
    (const char*[]){ "----#----", "----#----", "----#----", "----#----" },
    (const char*[]){ "---###---", "-#--#--#-", "---###---", "-#--#--#-" },
    (const char*[]){ "-#-###---", "-#--##-#-", "---###-#-", "-#-##--#-" },
};

const int g_maxPieces = 3;
const int g_maxRotations = 4;

const int g_pieceStrW = 3;
const int g_pieceStrH = 3;

void setRenderColorFromBlock(SDL_Renderer *renderer, char n);

Board::Board(int tileSize) : m_tileSize(tileSize) {
    putCurrentPiece();
}

void Board::update() {
    clearCurrentPiece();

    // Destroy blocks
    for (int y = 0; y < g_dim; y++) {
        bool full = true;
        for (int x = 0; x < g_dim; x++)
            if(m_board[y][x] == 0) {
                full = false;
                break;
            }

        if(full)
            for (int cy = y - 1; cy >= 0; cy--) {
                for (int cx = 0; cx < g_dim; cx++) {
                    m_board[cy + 1][cx] = m_board[cy][cx];
                }
            }
    }

    // Move Piece
    ticks++;
    if(ticks == g_ticksForMove) {
        ticks = 0;

        m_currentPieceY += 1;
        if(isCurrentPieceColliding()) {
            m_currentPieceY -= 1;
            putCurrentPiece();

            m_currentPiece++;
            if (m_currentPiece == g_maxPieces)
                m_currentPiece = 0;

            m_currentPieceX = g_dim/2;
            m_currentPieceY = 1;
            m_currentRotation = 0;
            return;
        }
    }

    m_currentRotation += m_dtCurrentRotation;
    if (m_currentRotation == -1)
        m_currentRotation = g_maxRotations - 1;
    if (m_currentRotation == g_maxRotations)
        m_currentRotation = 0;

    m_currentPieceX += m_dtCurrentPieceX;

    if(isCurrentPieceColliding()) {
        m_currentRotation -= m_dtCurrentRotation;
        if (m_currentRotation == -1)
            m_currentRotation = g_maxRotations - 1;
        if (m_currentRotation == g_maxRotations)
            m_currentRotation = 0;

        m_currentPieceX -= m_dtCurrentPieceX;
    }

    m_dtCurrentPieceX = 0;
    m_dtCurrentRotation = 0;

    putCurrentPiece();
}

void Board::draw(SDL_Renderer *renderer) {
    for (int y = 0; y < g_dim; y++) {
        for (int x = 0; x < g_dim; x++) {
            if (m_board[y][x] != 0) {
                setRenderColorFromBlock(renderer, m_board[y][x]);
                SDL_Rect tile = { x * m_tileSize, y * m_tileSize, m_tileSize, m_tileSize };
                SDL_RenderFillRect(renderer, &tile);
            }
       }
    }
}

void Board::putCurrentPiece() {
    const char *piece_str = pieces[m_currentPiece][m_currentRotation];

    for (int sy = 0; sy < g_pieceStrH; sy++) {
        for (int sx = 0; sx < g_pieceStrW; sx++) {
            if (piece_str[sy * g_pieceStrH + sx] == '#')
                m_board[m_currentPieceY + sy - 1][m_currentPieceX + sx - 1] = m_currentPiece + 1; /* NOTE: +1 because 0 is reserved */
        }
    }
}

void Board::clearCurrentPiece() {
    const char *piece_str = pieces[m_currentPiece][m_currentRotation];

    for (int sy = 0; sy < g_pieceStrH; sy++) {
        for (int sx = 0; sx < g_pieceStrW; sx++) {
            if (piece_str[sy * g_pieceStrH + sx] == '#')
                m_board[m_currentPieceY + sy - 1][m_currentPieceX + sx - 1] = 0;
        }
    }
}

void Board::changeCurrentRotation(int rotation) {
    m_dtCurrentRotation = rotation;
}

bool Board::isCurrentPieceColliding() {
    const char *piece_str = pieces[m_currentPiece][m_currentRotation];

    for (int sy = 0; sy < g_pieceStrH; sy++) {
        for (int sx = 0; sx < g_pieceStrW; sx++) {
            if (piece_str[sy * g_pieceStrH + sx] == '#')
                if (m_currentPieceY + sy - 1 >= g_dim || m_currentPieceY + sy - 1 < 0)
                    return true;
                else if (m_currentPieceX + sx - 1 >= g_dim || m_currentPieceX + sx - 1 < 0)
                    return true;
                else if (m_board[m_currentPieceY + sy - 1][m_currentPieceX + sx - 1] != 0)
                    return true;
        }
    }

    return false;
}

void setRenderColorFromBlock(SDL_Renderer *renderer, char n) {
    switch (n) {
    case 1:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        break;
    case 2:
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
        break;
    case 3:
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
        break;
    default:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        break;
    }
}

void Board::changeCurrentPieceX(int change) {
    m_dtCurrentPieceX = change;
}