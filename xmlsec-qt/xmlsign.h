#ifndef  QXMLSIGN_H
# define QXMLSIGN_H

# include "xmlsec-qt_global.h"
# include <QObject>
# include <QDomDocument>

class XMLSECQT_EXPORT QXmlSign : public QObject
{
public:
  explicit QXmlSign(QObject* parent = nullptr);
  virtual ~QXmlSign() {}

  QXmlSign& useDocument(const QDomDocument& doc) { _document = doc; return *this; }
  QXmlSign& useNamespace(const QString& ns) { _nspace = ns; return *this; }

  QString toString() const { return _document.toString(1); }
  const QDomDocument& document() const { return _document; }
  bool sign(const QString& keyFile);
  bool sign(const QString& xmlFile, const QString& keyFile);

protected:
  virtual void prepareDocument();

  QString     createTagName(const QString& tagName) const;
  QDomElement createElement(const QString& tagName);
  void        setXmlnsAttribute(QDomElement&, const QString& xmlns);
  QDomElement makeSignedInfo();
  QDomElement makeSignedInfoReference(const QStringList& transforms);
  QDomElement makeTransform(const QString& algorithm);
  QDomElement makeXPathTransform(const QString& algorithm);
  QDomElement makeKeyInfo();

  QDomDocument _document;
  QString      _nspace;
};

#endif
