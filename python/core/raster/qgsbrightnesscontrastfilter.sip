class QgsBrightnessContrastFilter : QgsRasterInterface
{
%TypeHeaderCode
#include <qgsbrightnesscontrastfilter.h>
%End
  public:
    QgsBrightnessContrastFilter( QgsRasterInterface *input = 0 );
    ~QgsBrightnessContrastFilter();

    virtual QgsBrightnessContrastFilter * clone() const /Factory/;

    int bandCount() const;

    Qgis::DataType dataType( int bandNo ) const;

    bool setInput( QgsRasterInterface* input );

    QgsRasterBlock *block( int bandNo, const QgsRectangle &extent, int width, int height, QgsRasterBlockFeedback* feedback = nullptr ) /Factory/;

    void setBrightness( int brightness );
    int brightness() const;

    void setContrast( int contrast );
    int contrast() const;

    void writeXml( QDomDocument& doc, QDomElement& parentElem ) const;

    /** Sets base class members from xml. Usually called from create() methods of subclasses*/
    void readXml( const QDomElement& filterElem );
};
