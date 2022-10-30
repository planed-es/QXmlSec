#include "xmlsignreference.h"

QXmlSignReference::QXmlSignReference()
{
}

QXmlSignReference& QXmlSignReference::useAlgorithm(const QUrl& url)
{
  algorithm = url;
  return *this;
}

QXmlSignReference& QXmlSignReference::addTransform(QUrl algorithm)
{
  if (algorithm == QUrl("http://www.w3.org/TR/1999/REC-xpath-19991116"))
    return addTransform(std::bind(&QXmlSignReference::makeXPathTransform, std::placeholders::_1, algorithm));
  return addTransform(std::bind(&QXmlSignReference::makeTransform, std::placeholders::_1, algorithm));
}

QXmlSignReference& QXmlSignReference::withUri(const QString& uri)
{
  hasUri = true;
  this->uri = uri;
  return *this;
}

QDomElement QXmlSignReference::generate(QXmlSignContext& context) const
{
  QDomElement root          = context.createElement("Reference");
  QDomElement digestEl      = context.createElement("DigestMethod");
  QDomElement digestValueEl = context.createElement("DigestValue");
  QString typeName = type.toString();

  digestEl.setAttribute("Algorithm", algorithm.toString());
  if (hasUri)
    root.setAttribute("URI", uri);
  if (id.length() > 0)
    root.setAttribute("Id", id);
  if (typeName.length() > 0)
    root.setAttribute("Type", typeName);
  if (transforms.size() > 0)
    root.appendChild(generateTransforms(context));
  root.appendChild(digestEl);
  root.appendChild(digestValueEl);
  return root;
}

QDomElement QXmlSignReference::generateTransforms(QXmlSignContext& context) const
{
  QDomElement root = context.createElement("Transforms");

  for (auto transform : transforms)
    root.appendChild(transform(context));
  return root;
}

QDomElement QXmlSignReference::makeXPathTransform(QXmlSignContext& context, const QUrl& algorithm)
{
  QDomElement transform = makeTransform(context, algorithm.toString());
  QDomElement xpathEl = context.createElement("XPath");
  QString     selector("not(ancestor-or-self::" + context.createTagName("Signature") + ')');

  xpathEl.setAttribute("xmlns", "http://www.w3.org/2000/09/xmldsig#");
  xpathEl.appendChild(context.document.createTextNode(selector));
  transform.appendChild(xpathEl);
  return transform;
}

QDomElement QXmlSignReference::makeTransform(QXmlSignContext& context, const QUrl& algorithm)
{
  QDomElement element = context.createElement("Transform");

  element.setAttribute("Algorithm", algorithm.toString());
  return element;
}
