#ifndef  XMLSECQTXADESSIGNATUREPOLICYQUALIFIER_H
# define XMLSECQTXADESSIGNATUREPOLICYQUALIFIER_H

# include "context.h"

class XMLSECQT_EXPORT QXadesSignaturePolicyQualifier
{
public:
  QXadesSignaturePolicyQualifier& useUrl(const QUrl& value) { spuri = value; return *this; }
  // TODO: implement SPUserNoticeType

  QDomElement generate(QXadesContext& context) const
  {
    QDomElement root = context.createElement("SigPolicyQualifier");

    if (!spuri.isEmpty())
    {
      QDomElement spuriEl = context.createElement("SPURI");

      spuriEl.appendChild(context.createTextNode(spuri.toString()));
      root.appendChild(spuriEl);
    }
    return root;
  }

private:
  QUrl spuri;
};

#endif
