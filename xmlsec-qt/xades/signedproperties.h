#ifndef  XMLSECQTXADESSIGNEDPROPERTIES_H
# define XMLSECQTXADESSIGNEDPROPERTIES_H

# include "signedsignatureproperties.h"
# include "dataobjectformat.h"
# include <QDateTime>

class XMLSECQT_EXPORT QXadesSignedProperties
{
public:
  QXadesSignedProperties() {}

  QXadesSignedSignatureProperties& signatureProperties() { return m_signatureProperties; }

  QXadesSignedProperties& addDataObjectFormat(const QXadesDataObjectFormat& value)
  {
    m_dataObjectFormats << value;
    return *this;
  }

  QDomElement generate(QXadesContext& context) const;
private:
  QDomElement generateSignedDataObjectProperties(QXadesContext& context) const;

  QXadesSignedSignatureProperties m_signatureProperties;
  QList<QXadesDataObjectFormat> m_dataObjectFormats;
};

#endif
