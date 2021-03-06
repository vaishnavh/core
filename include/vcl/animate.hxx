/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#ifndef INCLUDED_VCL_ANIMATE_HXX
#define INCLUDED_VCL_ANIMATE_HXX

#include <vcl/dllapi.h>
#include <vcl/timer.hxx>
#include <vcl/bitmapex.hxx>

#define ANIMATION_TIMEOUT_ON_CLICK 2147483647L

enum Disposal
{
    DISPOSE_NOT,
    DISPOSE_BACK,
    DISPOSE_FULL,
    DISPOSE_PREVIOUS
};

enum CycleMode
{
    CYCLE_NOT,
    CYCLE_NORMAL,
    CYCLE_FALLBACK,
    CYCLE_REVERS,
    CYCLE_REVERS_FALLBACK
};

struct VCL_DLLPUBLIC AnimationBitmap
{
    BitmapEx    aBmpEx;
    Point       aPosPix;
    Size        aSizePix;
    long        nWait;
    Disposal    eDisposal;
    bool    bUserInput;

                AnimationBitmap()
                    : nWait(0)
                    , eDisposal(DISPOSE_NOT)
                    , bUserInput(false)
                {
                }

                AnimationBitmap(
                    const BitmapEx& rBmpEx,
                    const Point& rPosPix,
                    const Size& rSizePix,
                    long _nWait = 0L,
                    Disposal _eDisposal = DISPOSE_NOT
                ) :
                    aBmpEx      ( rBmpEx ),
                    aPosPix     ( rPosPix ),
                    aSizePix    ( rSizePix ),
                    nWait       ( _nWait ),
                    eDisposal   ( _eDisposal ),
                    bUserInput  ( false )
                {}

    bool    operator==( const AnimationBitmap& rAnimBmp ) const
                {
                    return( rAnimBmp.aBmpEx == aBmpEx &&
                            rAnimBmp.aPosPix == aPosPix &&
                            rAnimBmp.aSizePix == aSizePix &&
                            rAnimBmp.nWait == nWait &&
                            rAnimBmp.eDisposal == eDisposal &&
                            rAnimBmp.bUserInput == bUserInput );
                }

    bool    operator!=( const AnimationBitmap& rAnimBmp ) const
                { return !( *this == rAnimBmp ); }

    bool    IsEqual( const AnimationBitmap& rAnimBmp ) const
                {
                    return( rAnimBmp.aPosPix == aPosPix &&
                            rAnimBmp.aSizePix == aSizePix &&
                            rAnimBmp.nWait == nWait &&
                            rAnimBmp.eDisposal == eDisposal &&
                            rAnimBmp.bUserInput == bUserInput &&
                            rAnimBmp.aBmpEx.IsEqual( aBmpEx ) );
                }

    sal_uLong   GetChecksum() const;
};

struct AInfo
{
    Bitmap          aLastSaveBitmap;
    Bitmap          aBackBitmap;
    Rectangle       aClipRect;
    Size            aLastSaveSize;
    Point           aLastSavePoint;
    Point           aStartOrg;
    Size            aStartSize;
    OutputDevice*   pOutDev;
    void*           pViewData;
    long            nExtraData;
    bool        bWithSize;
    bool        bPause;

                    AInfo() : pOutDev( NULL ),
                              pViewData( NULL ),
                              nExtraData( 0L ),
                              bWithSize( false ),
                              bPause( false ) {}
};

class ImplAnimView;
typedef ::std::vector< AnimationBitmap* > AnimationBitmapList_impl;
typedef ::std::vector< ImplAnimView* > AnimViewList_impl;

class VCL_DLLPUBLIC Animation
{
    SAL_DLLPRIVATE static sal_uLong     mnAnimCount;

    AnimationBitmapList_impl    maList;
    AnimViewList_impl           maViewList;
    Link                    maNotifyLink;
    BitmapEx                maBitmapEx;
    Timer                   maTimer;
    Size                    maGlobalSize;
    long                    mnLoopCount;
    long                    mnLoops;
    size_t                  mnPos;
    CycleMode               meCycleMode;
    bool                mbIsInAnimation;
    bool                mbLoopTerminated;
    bool                mbIsWaiting;


    SAL_DLLPRIVATE void     ImplRestartTimer( sal_uLong nTimeout );
    DECL_DLLPRIVATE_LINK(   ImplTimeoutHdl, void* );

public:

    SAL_DLLPRIVATE static void  ImplIncAnimCount() { mnAnimCount++; }
    SAL_DLLPRIVATE static void  ImplDecAnimCount() { mnAnimCount--; }
    SAL_DLLPRIVATE sal_uLong    ImplGetCurPos() const { return mnPos; }


public:
                            Animation();
                            Animation( const Animation& rAnimation );
                            ~Animation();

    Animation&              operator=( const Animation& rAnimation );
    bool                operator==( const Animation& rAnimation ) const;
    bool                operator!=( const Animation& rAnimation ) const
                            { return !(*this==rAnimation); }

    void                    Clear();

    bool                Start(
                                OutputDevice* pOutDev,
                                const Point& rDestPt,
                                const Size& rDestSz,
                                long nExtraData = 0,
                                OutputDevice* pFirstFrameOutDev = NULL
                            );
    void                    Stop( OutputDevice* pOutDev = NULL, long nExtraData = 0 );

    void                    Draw( OutputDevice* pOutDev, const Point& rDestPt ) const;
    void                    Draw( OutputDevice* pOutDev, const Point& rDestPt, const Size& rDestSz ) const;

    bool                IsInAnimation() const { return mbIsInAnimation; }
    bool                IsTransparent() const;
    bool                IsTerminated() const { return mbLoopTerminated; }

    const Size&             GetDisplaySizePixel() const { return maGlobalSize; }
    void                    SetDisplaySizePixel( const Size& rSize ) { maGlobalSize = rSize; }

    const BitmapEx&         GetBitmapEx() const { return maBitmapEx; }
    void                    SetBitmapEx( const BitmapEx& rBmpEx ) { maBitmapEx = rBmpEx; }

    sal_uLong               GetLoopCount() const { return mnLoopCount; }
    void                    SetLoopCount( const sal_uLong nLoopCount );
    void                    ResetLoopCount();

    void                    SetCycleMode( CycleMode eMode );
    CycleMode               GetCycleMode() const { return meCycleMode; }

    void                    SetNotifyHdl( const Link& rLink ) { maNotifyLink = rLink; }
    const Link&             GetNotifyHdl() const { return maNotifyLink; }

    size_t                  Count() const { return maList.size(); }
    bool                Insert( const AnimationBitmap& rAnimationBitmap );
    const AnimationBitmap&  Get( sal_uInt16 nAnimation ) const;
    void                    Replace( const AnimationBitmap& rNewAnimationBmp, sal_uInt16 nAnimation );

    sal_uLong               GetSizeBytes() const;
    sal_uLong               GetChecksum() const;

public:

    bool                Convert( BmpConversion eConversion );
    bool                ReduceColors(
                                sal_uInt16 nNewColorCount,
                                BmpReduce eReduce = BMP_REDUCE_SIMPLE
                            );
    bool                Invert();
    bool                Mirror( sal_uLong nMirrorFlags );
    bool                Adjust(
                                short nLuminancePercent = 0,
                                short nContrastPercent = 0,
                                short nChannelRPercent = 0,
                                short nChannelGPercent = 0,
                                short nChannelBPercent = 0,
                                double fGamma = 1.0,
                                bool bInvert = false
                            );
    bool                Filter(
                                BmpFilter eFilter,
                                const BmpFilterParam* pFilterParam = NULL,
                                const Link* pProgress = NULL
                            );

    friend VCL_DLLPUBLIC SvStream&  ReadAnimation( SvStream& rIStream, Animation& rAnimation );
    friend VCL_DLLPUBLIC SvStream&  WriteAnimation( SvStream& rOStream, const Animation& rAnimation );
};

#endif // INCLUDED_VCL_ANIMATE_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
