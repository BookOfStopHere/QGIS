/***************************************************************************
  qgsfieldformatter.cpp - QgsFieldFormatter

 ---------------------
 begin                : 2.12.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qgsfieldformatter.h"

#include "qgsfields.h"
#include "qgsvectorlayer.h"
#include "qgsvectordataprovider.h"

QgsFieldFormatter::QgsFieldFormatter()
{
}

QgsFieldFormatter::~QgsFieldFormatter()
{
}

QString QgsFieldFormatter::representValue( QgsVectorLayer* layer, int fieldIndex, const QVariantMap& config, const QVariant& cache, const QVariant& value ) const
{
  Q_UNUSED( layer )
  Q_UNUSED( fieldIndex )
  Q_UNUSED( config )
  Q_UNUSED( cache )
  Q_UNUSED( value )

  QString defVal;
  if ( layer->fields().fieldOrigin( fieldIndex ) == QgsFields::OriginProvider && layer->dataProvider() )
    defVal = layer->dataProvider()->defaultValueClause( layer->fields().fieldOriginIndex( fieldIndex ) );

  return value == defVal ? defVal : layer->fields().at( fieldIndex ).displayString( value );
}

QVariant QgsFieldFormatter::sortValue( QgsVectorLayer* layer, int fieldIndex, const QVariantMap& config, const QVariant& cache, const QVariant& value ) const
{
  Q_UNUSED( layer )
  Q_UNUSED( fieldIndex )
  Q_UNUSED( config )
  Q_UNUSED( cache )

  return value;
}

Qt::AlignmentFlag QgsFieldFormatter::alignmentFlag( QgsVectorLayer* layer, int fieldIndex, const QVariantMap& config ) const
{
  Q_UNUSED( config );

  QVariant::Type fldType = layer->fields().at( fieldIndex ).type();
  bool alignRight = ( fldType == QVariant::Int || fldType == QVariant::Double || fldType == QVariant::LongLong
                      || fldType == QVariant::DateTime || fldType == QVariant::Date || fldType == QVariant::Time );

  if ( alignRight )
    return Qt::AlignRight;
  else
    return Qt::AlignLeft;
}

QVariant QgsFieldFormatter::createCache( QgsVectorLayer* layer, int fieldIndex, const QVariantMap& config ) const
{
  Q_UNUSED( layer )
  Q_UNUSED( fieldIndex )
  Q_UNUSED( config )

  return QVariant();
}
