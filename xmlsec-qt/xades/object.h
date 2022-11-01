#ifndef  XMLSECQTXADESOBJECT_H
# define XMLSECQTXADESOBJECT_H

# include "../xmlsigncontext.h"
# include "signedproperties.h"

class XMLSECQT_EXPORT QXadesObject
{
public:
  explicit QXadesObject(QXmlSignContext parent);

  const QString& id() const { return m_id; }
  const QString& signedPropertiesId() const { return m_signedPropertiesId; }

  QXadesSignedProperties& signedProperties() { return m_signedProperties; }

  QXadesObject& useNamespace(const QString& value) { context.nspace = value; return *this; }

  QDomElement generate();
private:
  QDomElement generateSignedProperties();
  QDomElement generateUnsignedProperties();

  QXadesContext context;
  QXadesSignedProperties m_signedProperties;
  QString m_id, m_signedPropertiesId;
};

#endif
