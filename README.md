# QXmlSec

QXmlSec is a simple Qt library helping you implement the
[XML Signature Syntax](https://www.w3.org/TR/xmldsig-core/),
[XML Encryption Syntax](https://www.w3.org/TR/xmlenc-core/)
and [XML Advanced Electronic Signatures](https://www.w3.org/TR/XAdES/) (XAdES),
by leveraing Qt's [QDomDocument](https://doc.qt.io/qt-6/qdomdocument.html) and the
[xmlsec](https://www.aleksey.com/xmlsec/) library.

# Usage

## Signing a document

### xmldsig

```
#include <xmlsec-qt/xmlsign.h>
#include <iostream>

int main()
{
  QDomDocument targetDocument;
  QXmlSecCertificate certificateData;
  QXmlSign signer;
  QString keyInfoId;

  certificateData.setFormat(QXmlSecCertificate::Pkcs12);
  certificateData.setFilepath("./certificate.pfx");
  certificateData.setPassword("password");

  signer.withSignatureId("Signature-a53a6ab2")
        .useNamespace("ds")
        .useCertificate(certificateData)
        .useDocument(targetDocument);

  keyInfoId = signer.signatureContext().tagId("KeyInfo");

  signer.useKeyInfo(QXmlSignKeyInfo()
    .withId(keyInfoId)
    .withKeyValue()
    .withX509Data()
  );

  signer.useSignInfo(QXmlSignInfo()
    .addReference(QXmlSignReference()
      .withType(QUrl("http://www.w3.org/2000/09/xmldsig#Object"))
      .useAlgorithm(QUrl(http://www.w3.org/2000/09/xmldsig#Object))
      .addTransform(QUrl("http://www.w3.org/2000/09/xmldsig#enveloped-signature"))
      .addTransform(QUrl("http://www.w3.org/TR/1999/REC-xpath-19991116"))
    )
    .addReference(QXmlSignReference()
      .withUri('#' + keyInfoId)
      .useAlgorithm(QUrl("http://www.w3.org/2001/04/xmlenc#sha512"))
    )
  );

  if (signer.sign())
  {
    std::cerr << "Successfully signed XML document" << std::endl;
    std::cout << signer.toString().toStdString() << std::endl;
  }
  else
    std::cerr << "Failed to sign XML document." << std::endl;
  return 0;
}
```

The previous example would generate the following signature, envelopped within `targetDocument`:

```
<ds:Signature xmlns:ds="http://www.w3.org/2000/09/xmldsig#" Id="Signature-a53a6ab2">
    <ds:SignedInfo>
        <ds:CanonicalizationMethod Algorithm="http://www.w3.org/TR/2001/REC-xml-c14n-20010315"/>
        <ds:SignatureMethod Algorithm="http://www.w3.org/2001/04/xmldsig-more#rsa-sha256"/>
        <ds:Reference Id="Reference-cf54e6a9-7bf2-4128-8cae-474a267a16a1" Type="http://www.w3.org/2000/09/xmldsig#Object" URI="">
            <ds:Transforms>
                <ds:Transform Algorithm="http://www.w3.org/TR/2001/REC-xml-c14n-20010315"/>
                <ds:Transform Algorithm="http://www.w3.org/2000/09/xmldsig#enveloped-signature"/>
                <ds:Transform Algorithm="http://www.w3.org/TR/1999/REC-xpath-19991116">
                    <ds:XPath xmlns:ds="http://www.w3.org/2000/09/xmldsig#">not(ancestor-or-self::ds:Signature)</ds:XPath>
                </ds:Transform>
            </ds:Transforms>
            <ds:DigestMethod Algorithm="http://www.w3.org/2001/04/xmlenc#sha512"/>
            <ds:DigestValue>...</ds:DigestValue>
        </ds:Reference>
        <ds:Reference URI="#Signature-a53a6ab2-bkmcloef-KeyInfo">
            <ds:DigestMethod Algorithm="http://www.w3.org/2001/04/xmlenc#sha512"/>
            <ds:DigestValue>...</ds:DigestValue>
        </ds:Reference>
    </ds:SignedInfo>
    <ds:SignatureValue>...</ds:SignatureValue>
    <ds:KeyInfo Id="Signature-a53a6ab2-bkmcloef-KeyInfo">
      <ds:X509Data>
          <ds:X509Certificate>...</ds:X509Certificate>
      </ds:X509Data>
      <ds:KeyValue>
          <ds:RSAKeyValue>
              <ds:Modulus>...</ds:Modulus>
              <ds:Exponent>...</ds:Exponent>
          </ds:RSAKeyValue>
      </ds:KeyValue>
    </ds:KeyInfo>
</ds:Signature>
```

You may also provide a `QSslKey` instead of `QXmlsecCertificate`:

```
QSslKey sslKey;

signer.useSslKey(sslKey, "password");
```

Note that while this allows you better integration with the rest of your Qt program,
some xmlsigd fields might not work when using this method. It is safer to use
the `QXmlsecCertificate` object.

### XAdES

TODO


## Verifying the signature on a document

```
#include <xmlsec-qt/xmlverify.h>
#include <QFile>

int main()
{
  QFile sslKeyFile("./ssl.key");
  QFile signedDocument("./document.xml");

  if (sslKeyFile.open(QIODevice::ReadOnly) &&
      signedDocument.open(QIODevice::ReadOnly))
  {
    QString password("password");
    QSslKey sslKey(sslKeyFile, QSsl::Rsa, QSsl::Pem, QSsl::PublicKey, password);
    bool valid;

    valid = QXmlVerify(sslKey)
      .usePassphrase(password)
      .verify(signedDocument.readAll());
    return valid ? 0 : -1;
  }
  return -1;
}
```
