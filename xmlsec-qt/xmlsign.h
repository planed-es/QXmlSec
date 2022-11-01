#ifndef  QXMLSIGN_H
# define QXMLSIGN_H

# include "xmlsec-qt_global.h"
# include "xmlsigninfo.h"
# include "xmlkeyinfo.h"
# include "certificate.h"
# include <QObject>
# include <QtNetwork/QSslKey>
# include <QtNetwork/QSslCertificate>

class XMLSECQT_EXPORT QXmlSign : public QObject
{
  enum CertificateType
  {
    NoCertificate,
    XmlsecCertificate,
    QtCertificate
  };
  struct Certificate
  {
    QXmlSecCertificate xmlsec;
    QSslCertificate    qt;
  };
public:
  explicit QXmlSign(QObject* parent = nullptr);
  virtual ~QXmlSign() {}

  QXmlSign& useCertificate(const QXmlSecCertificate& val) { sslCertificateType = XmlsecCertificate;  sslCertificate.xmlsec = val; return *this; }
  QXmlSign& useCertificate(const QSslCertificate& val) { sslCertificateType = QtCertificate; sslCertificate.qt = val; return *this; }
  QXmlSign& useSslKey(const QSslKey&, const QByteArray& = QByteArray());
  QXmlSign& useDocument(const QDomDocument& doc) { context.document = doc; return *this; }
  QXmlSign& useNamespace(const QString& ns) { context.nspace = ns; return *this; }
  QXmlSign& useSignInfo(const QXmlSignInfo& value) { signInfo = value; return *this; }
  QXmlSign& useKeyInfo(const QXmlSignKeyInfo& value) { keyInfo = value; return *this; }
  QXmlSign& withSignatureId(const QString& id) { context.signatureId = id; return *this; }
  QXmlSign& addObject(QDomElement el) { objects << el; return *this; }

  QString toString() const { return output.length() == 0 ? context.document.toString(-1) : output; }
  QString toString(unsigned int indent) const { return context.document.toString(indent); }
  const QDomDocument& document() const { return context.document; }
  bool sign();
  bool sign(const QString& xmlFile);

  QXmlSignContext signatureContext() { return context; }

protected:
  virtual void prepareDocument();

  QDomElement makeSignedInfo();
  QDomElement makeSignedInfoReference(const QStringList& transforms);
  QDomElement makeTransform(const QString& algorithm);
  QDomElement makeXPathTransform(const QString& algorithm);
  QDomElement makeKeyInfo();

  CertificateType    sslCertificateType = NoCertificate;
  Certificate        sslCertificate;
  QSslKey            sslKey;
  QByteArray         sslKeyPassphrase;
  QXmlSignContext    context;
  QXmlSignInfo       signInfo;
  QXmlSignKeyInfo    keyInfo;
  QString            output;
  QList<QDomElement> objects;
};

#endif
