#ifndef  XMLSECQTCERTIFICATE_H
# define XMLSECQTCERTIFICATE_H

# include "xmlsec-qt_global.h"
# include <xmlsec/keysdata.h>

class XMLSECQT_EXPORT QXmlSecCertificate
{
public:
  enum Format
  {
    UnknownType   = xmlSecKeyDataFormatUnknown,
    Binary        = xmlSecKeyDataFormatBinary,
    Pem           = xmlSecKeyDataFormatPem,
    Der           = xmlSecKeyDataFormatDer,
    Pkcs8Pem      = xmlSecKeyDataFormatPkcs8Pem,
    Pkcs8Der      = xmlSecKeyDataFormatPkcs8Der,
    Pkcs12        = xmlSecKeyDataFormatPkcs12,
    PemCert       = xmlSecKeyDataFormatCertPem,
    DerCert       = xmlSecKeyDataFormatCertDer,
    OpenSSLEngine = xmlSecKeyDataFormatEngine
  };

  Format  format() const { return m_format; }
  void    setFormat(Format value) { m_format = value; }

  QString filepath() const { return m_filepath; }
  void    setFilepath(const QString& value) { m_filepath = value; }

  QString password() const { return m_password; }
  void    setPassword(const QString& value) { m_password = value; }

  QString name() const { return m_name; }
  void    setName(const QString& value) { m_name = value; }

  xmlSecKeyDataFormat xmlsecFormat() const
  {
    return static_cast<xmlSecKeyDataFormat>(m_format);
  }

private:
  Format  m_format;
  QString m_filepath;
  QString m_password;
  QString m_name;
};

#endif
