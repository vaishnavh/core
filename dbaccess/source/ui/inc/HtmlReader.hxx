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
#ifndef INCLUDED_DBACCESS_SOURCE_UI_INC_HTMLREADER_HXX
#define INCLUDED_DBACCESS_SOURCE_UI_INC_HTMLREADER_HXX

#include "DExport.hxx"
#include <svtools/parhtml.hxx>
#include <editeng/svxenum.hxx>
#include <tools/stream.hxx>
#include <com/sun/star/awt/FontDescriptor.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>

namespace dbaui
{
    class OHTMLReader : public HTMLParser, public ODatabaseExport
    {
        OUString            m_sCurrent;
        sal_Int32           m_nTableCount;
        sal_Int16           m_nWidth;
        sal_Int16           m_nColumnWidth; ///< maximum column width
        sal_Bool            m_bMetaOptions; ///< true when we scanned the meta information
        sal_Bool            m_bSDNum;

    protected:
        virtual void        NextToken( int nToken ) SAL_OVERRIDE; // base class
        virtual sal_Bool    CreateTable(int nToken) SAL_OVERRIDE;
        virtual TypeSelectionPageFactory
                            getTypeSelectionPageFactory() SAL_OVERRIDE;

        void                TableDataOn(SvxCellHorJustify& eVal);
        void                TableFontOn(::com::sun::star::awt::FontDescriptor& _rFont,sal_Int32 &_rTextColor);
        sal_Int16           GetWidthPixel( const HTMLOption& rOption );
        void                setTextEncoding();
        void                fetchOptions();
        virtual ~OHTMLReader();

    public:
        OHTMLReader(SvStream& rIn,
                    const SharedConnection& _rxConnection,
                    const ::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatter >& _rxNumberF,
                    const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext >& _rxContext,
                    const TColumnVector* rList = 0,
                    const OTypeInfoMap* _pInfoMap = 0);
        // required for automatic type recognition
        OHTMLReader(SvStream& rIn,
                    sal_Int32 nRows,
                    const TPositions &_rColumnPositions,
                    const ::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatter >& _rxNumberF,
                    const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext >& _rxContext,
                    const TColumnVector* rList,
                    const OTypeInfoMap* _pInfoMap,
                    sal_Bool _bAutoIncrementEnabled);

        virtual SvParserState CallParser() SAL_OVERRIDE;// base class
        virtual void release() SAL_OVERRIDE;
        ///< @attention recovers only valid data if 1. CTOR has been used
    };

    SV_DECL_IMPL_REF( OHTMLReader );
}
#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
