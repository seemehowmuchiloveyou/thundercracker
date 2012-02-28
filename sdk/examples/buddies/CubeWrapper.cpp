/* -*- mode: C; c-basic-offset: 4; intent-tabs-mode: nil -*-
 *
 * Copyright <c> 2012 Sifteo, Inc. All rights reserved.
 */

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CubeWrapper.h"
#include <sifteo/asset.h>
#include "Config.h"
#include "assets.gen.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Sifteo;

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace Buddies {

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace {

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const AssetImage &GetBuddyFaceBackgroundAsset(int buddyId)
{
    switch (buddyId)
    {
        default:
        case 0: return BuddyFaceBackground0;
        case 1: return BuddyFaceBackground1;
        case 2: return BuddyFaceBackground2;
        case 3: return BuddyFaceBackground3;
        case 4: return BuddyFaceBackground4;
        case 5: return BuddyFaceBackground5;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const AssetImage &GetBuddyFacePartsAsset(int buddyId)
{
    switch (buddyId)
    {
        default:
        case 0: return BuddyFaceParts0;
        case 1: return BuddyFaceParts1;
        case 2: return BuddyFaceParts2;
        case 3: return BuddyFaceParts3;
        case 4: return BuddyFaceParts4;
        case 5: return BuddyFaceParts5;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const Vec2 kPartPositions[NUM_SIDES] =
{
    Vec2(40,  0), // Sprites => Vec2(32, -8),
    Vec2( 0, 40), // Sprites => Vec2(-8, 32),
    Vec2(40, 80), // Sprites => Vec2(32, 72),
    Vec2(80, 40), // Sprites => Vec2(72, 32),
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CubeWrapper::CubeWrapper()
    : mCube()
    , mBg1Helper(mCube)
    , mEnabled(false)
    , mBuddyId(0)
    , mPieces()
    , mPiecesSolution()
    , mPieceOffsets()
    , mPieceAnimT(0.0f)
    , mPieceBlinking(SIDE_UNDEFINED)
    , mPieceBlinkTimer(0.0f)
    , mPieceBlinkingOn(false)
    , mCutsceneSpriteJumpRandom()
    , mCutsceneSpriteJump0(false)
    , mCutsceneSpriteJump1(false)
{
    for (unsigned int i = 0; i < NUM_SIDES; ++i)
    {
        mPieceOffsets[i] = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::Reset()
{
    for (unsigned int i = 0; i < NUM_SIDES; ++i)
    {
        mPieces[i] = Piece();
        mPiecesSolution[i] = Piece();
        mPieceOffsets[i] = 0;
    }
    mPieceAnimT = 0.0f;
    mPieceBlinking = SIDE_UNDEFINED;
    mPieceBlinkTimer = 0.0f;
    mPieceBlinkingOn = false;
    mCutsceneSpriteJump0 = false;
    mCutsceneSpriteJump1 = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::Update(float dt)
{
    mPieceAnimT += dt;
    while (mPieceAnimT > kPieceAnimPeriod)
    {
        mPieceAnimT -= kPieceAnimPeriod;
    }
    
    if (mPieceBlinking >= 0 && mPieceBlinking < NUM_SIDES)
    {
        ASSERT(mPieceBlinkTimer > 0.0f);
        
        mPieceBlinkTimer -= dt;
        if (mPieceBlinkTimer <= 0.0f)
        {
            mPieceBlinkingOn = !mPieceBlinkingOn;
            mPieceBlinkTimer += kHintBlinkTimerDuration;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawClear()
{
    Video().set();
    Video().clear();
    
    for (int i = 0; i < _SYS_VRAM_SPRITES; ++i)
    {
        Video().hideSprite(i);
    }
    
    mBg1Helper.Clear();
    mBg1Helper.Flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawFlush()
{
    mBg1Helper.Flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeWrapper::DrawNeedsSync()
{
    return mBg1Helper.NeedFinish();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawBuddy()
{
    ASSERT(IsEnabled());
    
    DrawBackground(GetBuddyFaceBackgroundAsset(mBuddyId));
    
    for (unsigned int i = 0; i < NUM_SIDES; ++i)
    {
        if (mPieceBlinking != int(i) || !mPieceBlinkingOn)
        {
            DrawPieceBg1(mPieces[i], i);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawBackground(const AssetImage &asset)
{
    Video().BG0_drawAsset(Vec2(0, 0), asset);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawUiAsset(
    const Vec2 &position,
    const AssetImage &asset, unsigned int assetFrame)
{
    mBg1Helper.DrawAsset(position, asset, assetFrame);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawUiText(
    const Vec2 &position,
    const AssetImage &assetFont,
    const char *text)
{
    ASSERT(text != NULL);
    mBg1Helper.DrawText(position, assetFont, text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::UpdateCutscene()
{
    UpdateCutsceneSpriteJump(mCutsceneSpriteJump0, 8, 1);
    UpdateCutsceneSpriteJump(mCutsceneSpriteJump1, 16, 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawCutscene(const char *text)
{
    ASSERT(text != NULL);
    ASSERT(2 <= _SYS_VRAM_SPRITES);
    
    Video().BG0_drawAsset(Vec2(0, 0), CutsceneBackground);
    
    Video().setSpriteImage(0, CutsceneSprites, 0);
    Video().setSpriteImage(1, CutsceneSprites, 1);
    
    Video().moveSprite(0, Vec2( 0, mCutsceneSpriteJump0 ? 64 : 72));
    Video().moveSprite(1, Vec2(64, mCutsceneSpriteJump1 ? 64 : 72));
    
    mBg1Helper.DrawAsset(Vec2(0, 0), CutsceneTextBubble);
    mBg1Helper.DrawText(Vec2(1, 1), Font, text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeWrapper::IsLoadingAssets()
{
    ASSERT(IsEnabled());
    
    return mCube.assetDone(GameAssets);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::LoadAssets()
{
    ASSERT(IsEnabled());
    
    mCube.loadAssets(GameAssets);
    
    VidMode_BG0_ROM rom(mCube.vbuf);
    rom.init();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawLoadingAssets()
{
    ASSERT(IsEnabled());
    
    VidMode_BG0_ROM rom(mCube.vbuf);
    rom.BG0_progressBar(Vec2(0, 0), mCube.assetProgress(GameAssets, VidMode_BG0::LCD_width), 16);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

Cube::ID CubeWrapper::GetId()
{
    return mCube.id();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeWrapper::IsEnabled() const
{
    return mEnabled;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::Enable(Cube::ID cubeId, unsigned int buddyId)
{
    ASSERT(!IsEnabled());
    
    mCube.enable(cubeId);
    
    // This ensure proper video state is set, even if we have kLoadAssets = false.
    Video().setWindow(0, VidMode::LCD_height);
    
    mEnabled = true;
    mBuddyId = buddyId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::Disable()
{
    ASSERT(IsEnabled());
    
    mEnabled = false;
    mCube.disable();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const Piece &CubeWrapper::GetPiece(Cube::Side side) const
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    return mPieces[side];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::SetPiece(Cube::Side side, const Piece &piece)
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    mPieces[side] = piece;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const Piece &CubeWrapper::GetPieceSolution(Cube::Side side) const
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    return mPiecesSolution[side];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::SetPieceSolution(Cube::Side side, const Piece &piece)
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    mPiecesSolution[side] = piece;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

int CubeWrapper::GetPieceOffset(Cube::Side side) const
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    return mPieceOffsets[side];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::SetPieceOffset(Cube::Side side, int offset)
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    mPieceOffsets[side] = offset;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::StartPieceBlinking(Cube::Side side)
{
    mPieceBlinking = side;
    mPieceBlinkTimer = kHintBlinkTimerDuration;
    mPieceBlinkingOn = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::StopPieceBlinking()
{
    mPieceBlinking = SIDE_UNDEFINED;
    mPieceBlinkTimer = 0.0f;
    mPieceBlinkingOn = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeWrapper::IsSolved() const
{
    ASSERT(IsEnabled());
    
    for (unsigned int i = 0; i < arraysize(mPiecesSolution); ++i)
    {
        if (mPiecesSolution[i].mMustSolve)
        {
            if (mPieces[i].mBuddy != mPiecesSolution[i].mBuddy ||
                mPieces[i].mPart != mPiecesSolution[i].mPart)
            {
                return false;
            }
        }
    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeWrapper::IsTouching() const
{
#ifdef SIFTEO_SIMULATOR
    return mCube.touching();
#else
    return false;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

VidMode_BG0_SPR_BG1 CubeWrapper::Video()
{
    return VidMode_BG0_SPR_BG1(mCube.vbuf);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawPieceSprite(const Piece &piece, Cube::Side side)
{
    ASSERT(piece.mPart >= 0 && piece.mPart < NUM_SIDES);
    ASSERT(side >= 0 && side < NUM_SIDES);
    ASSERT((NUM_SIDES * 2) <= _SYS_VRAM_SPRITES);
    
    int spriteLayer0 = side;
    int spriteLayer1 = side + NUM_SIDES;
    
    if (piece.mAttribute == Piece::ATTR_FIXED)
    {
        //Video().setSpriteImage(spriteLayer0, BuddyFacePartFixed);
    }
    else
    {
        Video().hideSprite(spriteLayer0);
    }
    
    if (piece.mAttribute == Piece::ATTR_HIDDEN)
    {
        //Video().setSpriteImage(spriteLayer1, BuddyFacePartHidden);
    }
    else
    {
        int rotation = side - piece.mPart;
        if (rotation < 0)
        {
            rotation += NUM_SIDES;
        }
        
        //const PinnedAssetImage &asset = GetBuddyFacePartsAsset(piece.mBuddy);
        //unsigned int frame = (rotation * NUM_SIDES) + piece.mPart;
        
        //ASSERT(frame < asset.frames);
        //Video().setSpriteImage(spriteLayer1, asset, frame);
    }
    
    Vec2 point = kPartPositions[side];
    
    switch(side)
    {
        case SIDE_TOP:
        {
            point.y += mPieceOffsets[side];
            break;
        }
        case SIDE_LEFT:
        {
            point.x += mPieceOffsets[side];
            break;
        }
        case SIDE_BOTTOM:
        {
            point.y -= mPieceOffsets[side];
            break;
        }
        case SIDE_RIGHT:
        {
            point.x -= mPieceOffsets[side];
            break;
        }
    }
    
    ASSERT(kPieceAnimPeriod > 0.0f);
    float w = 2.0f * M_PI / kPieceAnimPeriod;
    float x = kPieceAnimX * cosf(w * mPieceAnimT);
    float y = kPieceAnimY * sinf(w * mPieceAnimT);
    
    point.x += int(x);
    point.y += int(y);
    
    Video().moveSprite(spriteLayer0, point);
    Video().moveSprite(spriteLayer1, point);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawPieceBg1(const Piece &piece, Cube::Side side)
{
    int rotation = side - piece.mPart;
    if (rotation < 0)
    {
        rotation += NUM_SIDES;
    }
    
    const AssetImage &asset = GetBuddyFacePartsAsset(piece.mBuddy);
    unsigned int frame = (rotation * NUM_SIDES) + piece.mPart;
    ASSERT(frame < asset.frames);
    
    Vec2 point = kPartPositions[side];
    
    switch(side)
    {
        case SIDE_TOP:
        {
            point.y += mPieceOffsets[side];
            break;
        }
        case SIDE_LEFT:
        {
            point.x += mPieceOffsets[side];
            break;
        }
        case SIDE_BOTTOM:
        {
            point.y -= mPieceOffsets[side];
            break;
        }
        case SIDE_RIGHT:
        {
            point.x -= mPieceOffsets[side];
            break;
        }
    }
    
    Float2 pointFloat(point.x, point.y);
    pointFloat.x /= float(VidMode::TILE);
    pointFloat.y /= float(VidMode::TILE);
    
    point.x = int(pointFloat.x);
    point.y = int(pointFloat.y);
    
    if (point.x < int(-asset.width))
    {
        point.x = -asset.width;
    }
    
    if (point.x > int(16 + asset.width))
    {
        point.x = 16 + asset.width;
    }
    
    if (point.y < int(-asset.height))
    {
        point.y = -asset.height;
    }
    
    if (point.y > int(16 + asset.height))
    {
        point.y = 16 + asset.height;
    }
    
    if (point.x > int(-asset.width) && point.x < 0)
    {
        int tilesX = -point.x;
        
        mBg1Helper.DrawPartialAsset(
            Vec2(0, point.y),
            Vec2(tilesX, 0),
            Vec2(asset.width - tilesX, asset.height),
            asset, frame);
    }
    else if (point.x < 16 && (point.x + asset.width) > 16)
    {
        int tilesX = (point.x + asset.width) - 16;
        
        mBg1Helper.DrawPartialAsset(
            Vec2(point.x, point.y),
            Vec2(0, 0),
            Vec2(asset.width - tilesX, asset.height),
            asset, frame);
    }
    else if (point.y > int(-asset.height) && point.y < 0)
    {
        int tilesY = -point.y;
        
        mBg1Helper.DrawPartialAsset(
            Vec2(point.x, 0),
            Vec2(0, tilesY),
            Vec2(asset.width, asset.height - tilesY),
            asset, frame);
    }
    else if (point.y < 16 && (point.y + asset.height) > 16)
    {
        int tilesY = (point.y + asset.height) - 16;
        
        mBg1Helper.DrawPartialAsset(
            Vec2(point.x, point.y),
            Vec2(0, 0),
            Vec2(asset.width, asset.height - tilesY),
            asset, frame);
    }    
    else if (point.x >= 0 && point.x < 16 && point.y >= 0 && point.y < 16)
    {
        mBg1Helper.DrawAsset(Vec2(point.x, point.y), asset, frame);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::UpdateCutsceneSpriteJump(bool &cutsceneSpriteJump, int upChance, int downChance)
{
    if (!cutsceneSpriteJump)
    {
        if (mCutsceneSpriteJumpRandom.randrange(upChance) == 0)
        {
            cutsceneSpriteJump = true;
        }
    }
    else
    {
        if (mCutsceneSpriteJumpRandom.randrange(downChance) == 0)
        {
            cutsceneSpriteJump = false;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

}
