#ifndef  QXMLSIGNKEYINFO_H
# define QXMLSIGNKEYINFO_H

# include "xmlsigncontext.h"

class XMLSECQT_EXPORT QXmlSignKeyInfo
{
public:
  explicit QXmlSignKeyInfo()
  {
  }

  QXmlSignKeyInfo& withId(const QString& val) { id = val; return *this; }
  QXmlSignKeyInfo& withKeyValue(bool val = true) { keyValueEnabled = val; return *this; }
  QXmlSignKeyInfo& withKeyName(bool val = true) { keyNameEnabled = val; return *this; }
  QXmlSignKeyInfo& withX509Data(bool val = true) { x509dataEnabled = val; return *this; }

  QDomElement generate(QXmlSignContext&);

private:
  bool keyNameEnabled = false;
  bool keyValueEnabled = false;
  bool x509dataEnabled = false;
  QString id;
};

#endif
