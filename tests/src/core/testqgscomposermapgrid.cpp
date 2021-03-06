/***************************************************************************
                         testqgscomposermapgrid.cpp
                         ----------------------
    begin                : August 2014
    copyright            : (C) 2014 by Nyall Dawosn, Marco Hugentobler
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsapplication.h"
#include "qgscomposition.h"
#include "qgsmultirenderchecker.h"
#include "qgscomposermap.h"
#include "qgscomposermapgrid.h"
#include "qgsfontutils.h"
#include <QObject>
#include "qgstest.h"

class TestQgsComposerMapGrid : public QObject
{
    Q_OBJECT

  public:
    TestQgsComposerMapGrid();

  private slots:
    void initTestCase();// will be called before the first testfunction is executed.
    void cleanupTestCase();// will be called after the last testfunction was executed.
    void init();// will be called before each testfunction is executed.
    void cleanup();// will be called after every testfunction.
    void grid(); //test if grid and grid annotation works
    void reprojected(); //test if reprojected grid works
    void crossGrid(); //test if grid "cross" mode works
    void markerGrid(); //test if grid "marker" mode works
    void frameOnly(); //test if grid "frame/annotation" mode works
    void zebraStyle(); //test zebra map border style
    void zebraStyleSides(); //test zebra border on certain sides
    void frameDivisions(); //test filtering frame divisions
    void annotationFilter(); //test filtering annotations
    void interiorTicks(); //test interior tick mode
    void interiorTicksAnnotated(); //test interior tick mode with annotations
    void exteriorTicks(); //test exterior tick mode
    void exteriorTicksAnnotated(); //test exterior tick mode with annotations
    void interiorExteriorTicks(); //test interior & exterior tick mode
    void interiorExteriorTicksAnnotated(); //test interior & exterior tick mode with annotations
    void lineBorder(); //test line border frame mode
    void lineBorderAnnotated(); //test line border frame with annotations
    void annotationFormats(); //various tests for annotation formats
    void descendingAnnotations(); //test descending annotation direction

  private:
    QgsComposition* mComposition;
    QgsComposerMap* mComposerMap;
    QgsMapSettings *mMapSettings;
    QString mReport;
};

TestQgsComposerMapGrid::TestQgsComposerMapGrid()
    : mComposition( 0 )
    , mComposerMap( 0 )
    , mMapSettings( 0 )
{
}

void TestQgsComposerMapGrid::initTestCase()
{
  QgsApplication::init();
  QgsApplication::initQgis();
  mMapSettings = new QgsMapSettings();

  mReport = QStringLiteral( "<h1>Composer Map Grid Tests</h1>\n" );
}

void TestQgsComposerMapGrid::cleanupTestCase()
{
  delete mMapSettings;

  QString myReportFile = QDir::tempPath() + "/qgistest.html";
  QFile myFile( myReportFile );
  if ( myFile.open( QIODevice::WriteOnly | QIODevice::Append ) )
  {
    QTextStream myQTextStream( &myFile );
    myQTextStream << mReport;
    myFile.close();
  }

  QgsApplication::exitQgis();
}

void TestQgsComposerMapGrid::init()
{
  QgsCoordinateReferenceSystem crs = QgsCoordinateReferenceSystem( 32633 );
  mMapSettings->setDestinationCrs( crs );
  mMapSettings->setCrsTransformEnabled( false );
  mComposition = new QgsComposition( *mMapSettings );
  mComposition->setPaperSize( 297, 210 ); //A4 landscape
  mComposerMap = new QgsComposerMap( mComposition, 20, 20, 200, 100 );
  mComposerMap->setFrameEnabled( true );
  mComposerMap->setBackgroundColor( QColor( 150, 100, 100 ) );
  mComposerMap->grid()->setAnnotationFont( QgsFontUtils::getStandardTestFont() );
  mComposerMap->grid()->setAnnotationPrecision( 0 );
  mComposerMap->grid()->setIntervalX( 2000 );
  mComposerMap->grid()->setIntervalY( 2000 );
  mComposerMap->grid()->setGridLineWidth( 0.5 );
  mComposerMap->grid()->setGridLineColor( QColor( 0, 0, 0 ) );
  mComposerMap->updateBoundingRect();
  mComposition->addComposerMap( mComposerMap );
}

void TestQgsComposerMapGrid::cleanup()
{
  delete mComposition;
}

void TestQgsComposerMapGrid::grid()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );
  mComposerMap->grid()->setEnabled( true );

  mComposerMap->grid()->setAnnotationEnabled( true );
  mComposerMap->grid()->setGridLineColor( QColor( 0, 255, 0 ) );
  mComposerMap->grid()->setAnnotationDisplay( QgsComposerMapGrid::HideAll, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationDisplay( QgsComposerMapGrid::HideAll, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->grid()->setAnnotationDirection( QgsComposerMapGrid::Horizontal, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationDirection( QgsComposerMapGrid::Horizontal, QgsComposerMapGrid::Bottom );
  mComposerMap->grid()->setAnnotationFontColor( QColor( 255, 0, 0, 150 ) );
  mComposerMap->grid()->setBlendMode( QPainter::CompositionMode_Overlay );
  mComposerMap->updateBoundingRect();
  qWarning() << "grid annotation font: " << mComposerMap->grid()->annotationFont().toString() << " exactMatch:" << mComposerMap->grid()->annotationFont().exactMatch();
  QgsCompositionChecker checker( QStringLiteral( "composermap_grid" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );

  bool testResult = checker.testComposition( mReport, 0, 0 );
  mComposerMap->grid()->setEnabled( false );
  mComposerMap->grid()->setAnnotationEnabled( false );
  QVERIFY( testResult );
}

void TestQgsComposerMapGrid::reprojected()
{
  mComposerMap->setNewExtent( QgsRectangle( -243577.565, 2939084.773, 1215622.435, 3668684.773 ) );
  QgsCoordinateReferenceSystem geographic = QgsCoordinateReferenceSystem( 4326 );
  mComposerMap->grid()->setCrs( geographic );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setIntervalX( 1 );
  mComposerMap->grid()->setIntervalY( 1 );
  mComposerMap->grid()->setAnnotationEnabled( false );
  mComposerMap->grid()->setBlendMode( QPainter::CompositionMode_SourceOver );
  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::ExteriorTicks );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->setFrameEnabled( false );
  mComposerMap->updateBoundingRect();
  QgsCompositionChecker checker( QStringLiteral( "composermap_gridreprojected" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );

  bool testResult = checker.testComposition( mReport, 0, 0 );
  mComposerMap->grid()->setEnabled( false );
  mComposerMap->grid()->setCrs( mMapSettings->destinationCrs() );
  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
  mComposerMap->setFrameEnabled( true );
  QVERIFY( testResult );
}

void TestQgsComposerMapGrid::crossGrid()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::Cross );
  mComposerMap->grid()->setCrossLength( 2.0 );
  mComposerMap->grid()->setAnnotationEnabled( false );
  mComposerMap->grid()->setGridLineColor( QColor( 0, 255, 0 ) );
  mComposerMap->grid()->setBlendMode( QPainter::CompositionMode_SourceOver );
  mComposerMap->updateBoundingRect();
  QgsCompositionChecker checker( QStringLiteral( "composermap_crossgrid" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );

  bool testResult = checker.testComposition( mReport, 0, 0 );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::Solid );
  mComposerMap->grid()->setEnabled( false );
  mComposerMap->grid()->setAnnotationEnabled( false );
  QVERIFY( testResult );
}

void TestQgsComposerMapGrid::markerGrid()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::Markers );
  mComposerMap->grid()->setAnnotationEnabled( false );
  mComposerMap->grid()->setBlendMode( QPainter::CompositionMode_SourceOver );
  mComposerMap->updateBoundingRect();
  QgsCompositionChecker checker( QStringLiteral( "composermap_markergrid" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );

  bool testResult = checker.testComposition( mReport, 0, 0 );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::Solid );
  mComposerMap->grid()->setEnabled( false );
  mComposerMap->grid()->setAnnotationEnabled( false );
  QVERIFY( testResult );
}

void TestQgsComposerMapGrid::frameOnly()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::FrameAnnotationsOnly );
  mComposerMap->grid()->setAnnotationEnabled( false );
  //set a frame for testing
  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::Zebra );
  mComposerMap->grid()->setFrameWidth( 2.0 );
  mComposerMap->grid()->setFramePenSize( 0.5 );
  mComposerMap->grid()->setBlendMode( QPainter::CompositionMode_SourceOver );
  mComposerMap->updateBoundingRect();
  QgsCompositionChecker checker( QStringLiteral( "composermap_gridframeonly" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );

  bool testResult = checker.testComposition( mReport, 0, 0 );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::Solid );
  mComposerMap->grid()->setEnabled( false );
  mComposerMap->grid()->setAnnotationEnabled( false );
  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
  QVERIFY( testResult );
}

void TestQgsComposerMapGrid::zebraStyle()
{
  mComposerMap->setNewExtent( QgsRectangle( 785462.375, 3341423.125, 789262.375, 3343323.125 ) ); //zoom in
  mComposerMap->grid()->setAnnotationFontColor( QColor( 0, 0, 0, 0 ) );
  mComposerMap->grid()->setBlendMode( QPainter::CompositionMode_SourceOver );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::Zebra );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( QColor( 255, 100, 0, 200 ) );
  mComposerMap->grid()->setFrameFillColor1( QColor( 50, 90, 50, 100 ) );
  mComposerMap->grid()->setFrameFillColor2( QColor( 200, 220, 100, 60 ) );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_zebrastyle" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );

  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );
}

void TestQgsComposerMapGrid::zebraStyleSides()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );
  mComposerMap->grid()->setAnnotationFontColor( QColor( 0, 0, 0, 0 ) );
  mComposerMap->grid()->setBlendMode( QPainter::CompositionMode_SourceOver );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::Zebra );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( Qt::black );
  mComposerMap->grid()->setFrameFillColor1( Qt::black );
  mComposerMap->grid()->setFrameFillColor2( Qt::white );
  mComposerMap->grid()->setEnabled( true );

  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameLeft, true );
  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameRight, false );
  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameTop, false );
  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameBottom, false );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_zebrastyle_left" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameTop, true );
  mComposerMap->updateBoundingRect();
  QgsCompositionChecker checker2( QStringLiteral( "composermap_zebrastyle_lefttop" ), mComposition );
  checker2.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult2 = checker2.testComposition( mReport, 0, 0 );
  QVERIFY( testResult2 );

  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameRight, true );
  mComposerMap->updateBoundingRect();
  QgsCompositionChecker checker3( QStringLiteral( "composermap_zebrastyle_lefttopright" ), mComposition );
  checker3.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult3 = checker3.testComposition( mReport, 0, 0 );
  QVERIFY( testResult3 );

  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameBottom, true );
  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );

}

void TestQgsComposerMapGrid::frameDivisions()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );
  //rotate map, so we mix latitude and longitude coordinates on every map side
  mComposerMap->setMapRotation( 45.0 );

  //setup defaults
  mComposerMap->grid()->setAnnotationFontColor( QColor( 0, 0, 0, 0 ) );
  mComposerMap->grid()->setBlendMode( QPainter::CompositionMode_SourceOver );
  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::Zebra );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( Qt::black );
  mComposerMap->grid()->setFrameFillColor1( Qt::black );
  mComposerMap->grid()->setFrameFillColor2( Qt::white );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameLeft, true );
  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameRight, true );
  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameTop, true );
  mComposerMap->grid()->setFrameSideFlag( QgsComposerMapGrid::FrameBottom, true );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_rotatedframe" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setFrameDivisions( QgsComposerMapGrid::LatitudeOnly, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setFrameDivisions( QgsComposerMapGrid::LongitudeOnly, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setFrameDivisions( QgsComposerMapGrid::LatitudeOnly, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setFrameDivisions( QgsComposerMapGrid::LongitudeOnly, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker2( QStringLiteral( "composermap_framedivisions" ), mComposition );
  checker2.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  testResult = checker2.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setFrameDivisions( QgsComposerMapGrid::ShowAll, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setFrameDivisions( QgsComposerMapGrid::ShowAll, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setFrameDivisions( QgsComposerMapGrid::ShowAll, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setFrameDivisions( QgsComposerMapGrid::ShowAll, QgsComposerMapGrid::Bottom );
  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
  mComposerMap->setMapRotation( 0.0 );
}

void TestQgsComposerMapGrid::annotationFilter()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );
  //rotate map, so we mix latitude and longitude coordinates on every map side
  mComposerMap->setMapRotation( 45.0 );

  //setup defaults
  mComposerMap->grid()->setAnnotationFontColor( QColor( 0, 0, 0, 0 ) );
  mComposerMap->grid()->setBlendMode( QPainter::CompositionMode_SourceOver );
  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setAnnotationEnabled( true );
  mComposerMap->grid()->setAnnotationFontColor( Qt::black );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_rotatedannotations" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setAnnotationDisplay( QgsComposerMapGrid::HideAll, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationDisplay( QgsComposerMapGrid::LongitudeOnly, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationDisplay( QgsComposerMapGrid::LatitudeOnly, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationDisplay( QgsComposerMapGrid::LongitudeOnly, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker2( QStringLiteral( "composermap_filteredannotations" ), mComposition );
  checker2.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  testResult = checker2.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setAnnotationEnabled( false );
  mComposerMap->grid()->setAnnotationDisplay( QgsComposerMapGrid::ShowAll, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationDisplay( QgsComposerMapGrid::ShowAll, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationDisplay( QgsComposerMapGrid::ShowAll, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationDisplay( QgsComposerMapGrid::ShowAll, QgsComposerMapGrid::Bottom );
  mComposerMap->setMapRotation( 0.0 );
}

void TestQgsComposerMapGrid::interiorTicks()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::InteriorTicks );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( Qt::black );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::FrameAnnotationsOnly );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_interiorticks" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
}

void TestQgsComposerMapGrid::interiorTicksAnnotated()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::InteriorTicks );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( Qt::black );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::FrameAnnotationsOnly );
  mComposerMap->grid()->setAnnotationEnabled( true );
  mComposerMap->grid()->setAnnotationFontColor( Qt::black );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_interiorticks_annotated" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker2( QStringLiteral( "composermap_interiorticks_annotated2" ), mComposition );
  checker2.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult2 = checker2.testComposition( mReport, 0, 0 );
  QVERIFY( testResult2 );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
  mComposerMap->grid()->setAnnotationEnabled( false );
}

void TestQgsComposerMapGrid::exteriorTicks()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::ExteriorTicks );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( Qt::black );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::FrameAnnotationsOnly );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_exteriorticks" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
}

void TestQgsComposerMapGrid::exteriorTicksAnnotated()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::ExteriorTicks );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( Qt::black );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::FrameAnnotationsOnly );
  mComposerMap->grid()->setAnnotationEnabled( true );
  mComposerMap->grid()->setAnnotationFontColor( Qt::black );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_exteriorticks_annotated" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker2( QStringLiteral( "composermap_exteriorticks_annotated2" ), mComposition );
  checker2.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult2 = checker2.testComposition( mReport, 0, 0 );
  QVERIFY( testResult2 );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
  mComposerMap->grid()->setAnnotationEnabled( false );
}

void TestQgsComposerMapGrid::interiorExteriorTicks()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::InteriorExteriorTicks );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( Qt::black );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::FrameAnnotationsOnly );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_interiorexteriorticks" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
}

void TestQgsComposerMapGrid::interiorExteriorTicksAnnotated()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::InteriorExteriorTicks );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( Qt::black );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::FrameAnnotationsOnly );
  mComposerMap->grid()->setAnnotationEnabled( true );
  mComposerMap->grid()->setAnnotationFontColor( Qt::black );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_interiorexteriorticks_annotated" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker2( QStringLiteral( "composermap_interiorexteriorticks_annotated2" ), mComposition );
  checker2.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult2 = checker2.testComposition( mReport, 0, 0 );
  QVERIFY( testResult2 );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
  mComposerMap->grid()->setAnnotationEnabled( false );
}

void TestQgsComposerMapGrid::lineBorder()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::LineBorder );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( Qt::black );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::FrameAnnotationsOnly );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_lineborder" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
}

void TestQgsComposerMapGrid::lineBorderAnnotated()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::LineBorder );
  mComposerMap->grid()->setFrameWidth( 10 );
  mComposerMap->grid()->setFramePenSize( 1 );
  mComposerMap->grid()->setFramePenColor( Qt::black );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::FrameAnnotationsOnly );
  mComposerMap->grid()->setAnnotationEnabled( true );
  mComposerMap->grid()->setAnnotationFontColor( Qt::black );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_lineborder_annotated" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker2( QStringLiteral( "composermap_lineborder_annotated2" ), mComposition );
  checker2.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult2 = checker2.testComposition( mReport, 0, 0 );
  QVERIFY( testResult2 );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
  mComposerMap->grid()->setAnnotationEnabled( false );
}

void TestQgsComposerMapGrid::annotationFormats()
{
  //create grids in geographic and projected coordinates
  QgsCoordinateReferenceSystem projectedCrs;
  projectedCrs.createFromSrid( 3994 );
  QgsCoordinateReferenceSystem geographicCrs;
  geographicCrs.createFromSrid( 4326 );

  QgsComposerMapGrid gridGeographic( QStringLiteral( "geographic grid" ), mComposerMap );
  gridGeographic.setCrs( geographicCrs );
  QgsComposerMapGrid gridProjected( QStringLiteral( "projected grid" ), mComposerMap );
  gridProjected.setCrs( projectedCrs );

  //decimal degrees format
  gridGeographic.setAnnotationFormat( QgsComposerMapGrid::DecimalWithSuffix );
  gridGeographic.setAnnotationPrecision( 1 );
  gridProjected.setAnnotationFormat( QgsComposerMapGrid::DecimalWithSuffix );
  gridProjected.setAnnotationPrecision( 1 );

  QgsExpressionContext expressionContext = gridGeographic.createExpressionContext();

  //normal e/w
  QCOMPARE( gridGeographic.gridAnnotationString( 90, QgsComposerMapGrid::Longitude, expressionContext ), QString( "90.0" ) + QChar( 176 ) + QString( "E" ) );
  QCOMPARE( gridProjected.gridAnnotationString( 90, QgsComposerMapGrid::Longitude, expressionContext ), QString( "90.0E" ) );

  //0 degrees
  QCOMPARE( gridGeographic.gridAnnotationString( 0, QgsComposerMapGrid::Longitude, expressionContext ), QString( "0.0" ) + QChar( 176 ) );
  QCOMPARE( gridProjected.gridAnnotationString( 0, QgsComposerMapGrid::Longitude, expressionContext ), QString( "0.0E" ) );

  //180 degrees
  QCOMPARE( gridGeographic.gridAnnotationString( 180, QgsComposerMapGrid::Longitude, expressionContext ), QString( "180.0" ) + QChar( 176 ) );
  QCOMPARE( gridProjected.gridAnnotationString( 180, QgsComposerMapGrid::Longitude, expressionContext ), QString( "180.0E" ) );

  //normal n/s
  QCOMPARE( gridGeographic.gridAnnotationString( 45, QgsComposerMapGrid::Latitude, expressionContext ), QString( "45.0" ) + QChar( 176 ) + QString( "N" ) );
  QCOMPARE( gridProjected.gridAnnotationString( 45, QgsComposerMapGrid::Latitude, expressionContext ), QString( "45.0N" ) );

  //0 north/south
  QCOMPARE( gridGeographic.gridAnnotationString( 0, QgsComposerMapGrid::Latitude, expressionContext ), QString( "0.0" ) + QChar( 176 ) );
  QCOMPARE( gridProjected.gridAnnotationString( 0, QgsComposerMapGrid::Latitude, expressionContext ), QString( "0.0N" ) );

  //Custom format annotations
  gridProjected.setAnnotationFormat( QgsComposerMapGrid::CustomFormat );
  gridProjected.setAnnotationExpression( QStringLiteral( "(@grid_number/10) || case when @grid_axis ='x' then 'a' else 'b' end" ) );
  QCOMPARE( gridProjected.gridAnnotationString( 45, QgsComposerMapGrid::Latitude, expressionContext ), QString( "4.5b" ) );
  QCOMPARE( gridProjected.gridAnnotationString( 33, QgsComposerMapGrid::Longitude, expressionContext ), QString( "3.3a" ) );
}

void TestQgsComposerMapGrid::descendingAnnotations()
{
  mComposerMap->setNewExtent( QgsRectangle( 781662.375, 3339523.125, 793062.375, 3345223.125 ) );

  mComposerMap->grid()->setFrameStyle( QgsComposerMapGrid::NoFrame );
  mComposerMap->grid()->setEnabled( true );
  mComposerMap->grid()->setStyle( QgsComposerMapGrid::FrameAnnotationsOnly );
  mComposerMap->grid()->setAnnotationEnabled( true );
  mComposerMap->grid()->setAnnotationFontColor( Qt::black );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::InsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->grid()->setAnnotationDirection( QgsComposerMapGrid::VerticalDescending, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationDirection( QgsComposerMapGrid::VerticalDescending, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationDirection( QgsComposerMapGrid::VerticalDescending, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationDirection( QgsComposerMapGrid::VerticalDescending, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker( QStringLiteral( "composermap_verticaldescending_inside" ), mComposition );
  checker.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult = checker.testComposition( mReport, 0, 0 );
  QVERIFY( testResult );

  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Left );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Right );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Top );
  mComposerMap->grid()->setAnnotationPosition( QgsComposerMapGrid::OutsideMapFrame, QgsComposerMapGrid::Bottom );
  mComposerMap->updateBoundingRect();

  QgsCompositionChecker checker2( QStringLiteral( "composermap_verticaldescending_outside" ), mComposition );
  checker2.setControlPathPrefix( QStringLiteral( "composer_mapgrid" ) );
  bool testResult2 = checker2.testComposition( mReport, 0, 0 );
  QVERIFY( testResult2 );

  mComposerMap->grid()->setAnnotationEnabled( false );
}

QGSTEST_MAIN( TestQgsComposerMapGrid )
#include "testqgscomposermapgrid.moc"
