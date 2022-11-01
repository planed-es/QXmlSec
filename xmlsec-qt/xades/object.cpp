#include "object.h"

QXadesObject::QXadesObject(QXmlSignContext parent) : context(parent)
{
  m_id = context.tagId("QualifyingProperties");
  m_signedPropertiesId = context.tagId("SignedProperties");
}

QDomElement QXadesObject::generate()
{
  QDomElement objectEl = context.xmlsec.createElement("Object");
  QDomElement root = context.createElement("QualifyingProperties");

  root.setAttribute("Id", id());
  root.setAttribute("Target", '#' + context.xmlsec.signatureId);
  context.xmlsec.setXmlnsAttribute(root);
  context.setXmlnsAttribute(root);
  root.appendChild(generateSignedProperties());
  root.appendChild(generateUnsignedProperties());
  objectEl.appendChild(root);
  return objectEl;
}

QDomElement QXadesObject::generateSignedProperties()
{
  QDomElement root = m_signedProperties.generate(context);

  root.setAttribute("Id", signedPropertiesId());
  return root;
}

QDomElement QXadesObject::generateUnsignedProperties()
{
  QDomElement root = context.createElement("UnsignedProperties");

  return root;
}
