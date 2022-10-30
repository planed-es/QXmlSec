#ifndef  QXMLSIGNREFERENCE_H
# define QXMLSIGNREFERENCE_H

# include "xmlsigncontext.h"
# include <QDomElement>
# include <functional>

class XMLSECQT_EXPORT QXmlSignReference
{
public:
  typedef std::function<QDomElement(QXmlSignContext&)> TransformFunction;

  explicit QXmlSignReference();

  QXmlSignReference& useAlgorithm(const QUrl& url);
  QXmlSignReference& addTransform(QUrl algorithm);
  QXmlSignReference& addTransform(TransformFunction func) { transforms.push_back(func); return *this; }
  QXmlSignReference& withType(const QUrl& url) { type = url; return *this; }
  QXmlSignReference& withUri(const QString& uri);
  QXmlSignReference& withId(const QString& val) { id = val; return *this; }

  QDomElement        generate(QXmlSignContext& context) const;
private:
  QDomElement        generateTransforms(QXmlSignContext& context) const;
  static QDomElement makeXPathTransform(QXmlSignContext& context, const QUrl& algorithm);
  static QDomElement makeTransform(QXmlSignContext& context, const QUrl& algorithm);

  QVector<TransformFunction> transforms;
  QUrl algorithm, type;
  bool hasUri = false;
  QString uri;
  QString id;
};

#endif
