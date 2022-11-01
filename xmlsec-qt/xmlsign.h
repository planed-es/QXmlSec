#ifndef  QXMLSIGN_H
# define QXMLSIGN_H

# include "xmlsec-qt_global.h"
# include "xmlsigninfo.h"
# include "xmlkeyinfo.h"
# include "certificate.h"
# include <QObject>

class XMLSECQT_EXPORT QXmlSign : public QObject
{
public:
  explicit QXmlSign(QObject* parent = nullptr);
  virtual ~QXmlSign() {}

  QXmlSign& useDocument(const QDomDocument& doc) { context.document = doc; return *this; }
  QXmlSign& useNamespace(const QString& ns) { context.nspace = ns; return *this; }
  QXmlSign& useSignInfo(const QXmlSignInfo& value) { signInfo = value; return *this; }
  QXmlSign& useKeyInfo(const QXmlSignKeyInfo& value) { keyInfo = value; return *this; }
  QXmlSign& withSignatureId(const QString& id) { context.signatureId = id; return *this; }
  QXmlSign& addObject(QDomElement el) { objects << el; return *this; }

  QString toString() const { return output.length() == 0 ? context.document.toString(-1) : output; }
  QString toString(unsigned int indent) const { return context.document.toString(indent); }
  const QDomDocument& document() const { return context.document; }
  bool sign(const QXmlSecCertificate&);
  bool sign(const QString& xmlFile, const QXmlSecCertificate&);

  QXmlSignContext signatureContext() { return context; }

protected:
  virtual void prepareDocument();

  QDomElement makeSignedInfo();
  QDomElement makeSignedInfoReference(const QStringList& transforms);
  QDomElement makeTransform(const QString& algorithm);
  QDomElement makeXPathTransform(const QString& algorithm);
  QDomElement makeKeyInfo();

  QXmlSignContext    context;
  QXmlSignInfo       signInfo;
  QXmlSignKeyInfo    keyInfo;
  QString            output;
  QList<QDomElement> objects;
};

#endif
