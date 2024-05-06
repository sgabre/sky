#!/bin/bash

# https://www.cs.toronto.edu/~arnold/427/19s/427_19S/tool/ssl/notes.pdf
# https://learn.microsoft.com/en-us/azure/application-gateway/self-signed-certificates
# https://www.digicert.com/kb/ssl-support/openssl-quick-reference-guide.htm

SERVER_DIR="Server"
SERVER_PLAIN_DIR=$SERVER_DIR/PlainData
SERVER_PLAIN_FILE=$SERVER_DIR/PlainData/Message.txt
SERVER_ENCRYPTED_FILE=$SERVER_DIR/Message.bin

SERVER_PRIVATE_KEY=$SERVER_DIR/ServerPrivateKey.pem
SERVER_PUBLIC_KEY=$SERVER_DIR/ServerPublicKey.pem
SERVER_CERTIFICAT_REQUEST=$SERVER_DIR/ServerCerificatRequest.csr
SERVER_CERTIFICAT=$SERVER_DIR/ServerCerificat.cst

SERVER_DEVICE_CERTIFICAT=$SERVER_DIR/DeviceCerificat.cst
SERVER_DEVICE_PUBLIC_KEY=$SERVER_DIR/DevicePublicKey.pem

SERVER_SYMMETRIC_KEY=$SERVER_DIR/UniqueSymmetric.pem
SERVER_ENCRYPTED_SYMMETRIC_KEY=$SERVER_DIR/UniqueSymmetric.enc
SERVER_SYMMETRIC_KEY_SIGNATURE=$SERVER_DIR/UniqueSymmetricSignature.bin

SERVER_COUNTRY=US
SERVER_STATE=Utah
SERVER_LOCALITY=Lehi
SERVER_ORGANIZATION_NAME=GSI
SERVER_ORGANIZATION_UNIT_NAME=IT
SERVER_COMMON_NAME=www.gsi.com
SERVER_EMAIL=godefroy.sieffert@gmail.com
SERVER_PASSWORD=




DEVICE_DIR="Device"
DEVICE_PRIVATE_KEY=$DEVICE_DIR/DevicePrivateKey.pem
DEVICE_PUBLIC_KEY=$DEVICE_DIR/DevicePublicKey.pem
DEVICE_CERTIFICAT_REQUEST=$DEVICE_DIR/DeviceCerificatRequest.csr
DEVICE_CERTIFICAT=$DEVICE_DIR/DeviceCerificat.cst

DEVICE_ENCRYPTED_SYMMETRIC_KEY=$DEVICE_DIR/UniqueSymmetric.enc
DEVICE_SYMMETRIC_KEY=$DEVICE_DIR/UniqueSymmetric.pem
DEVICE_SYMMETRIC_KEY_SIGNATURE=$DEVICE_DIR/UniqueSymmetricSignature.bin
DEVICE_SERVER_DEVICE_CERTIFICAT=$DEVICE_DIR/ServerCerificat.cst
DEVICE_SERVER_PUBLIC_KEY=$DEVICE_DIR/ServerPublicKey.pem


DEVICE_ENCRYPTED_FILE=$DEVICE_DIR/Message.bin
DEVICE_PLAIN_FILE=$DEVICE_DIR/Message.txt

DEVICE_COUNTRY=US
DEVICE_STATE=Utah
DEVICE_LOCALITY=Lehi
DEVICE_ORGANIZATION_NAME=GSI
DEVICE_ORGANIZATION_UNIT_NAME=IT
DEVICE_COMMON_NAME=www.gsi.com
DEVICE_EMAIL=godefroy.sieffert@gmail.com
DEVICE_PASSWORD=


CA_DIR="CertificateAuthority"

CA_PRIVATE_KEY=$CA_DIR/RootPrivateKey.pem
CA_PUBLIC_KEY=$CA_DIR/RootPublicKey.pem
CA_CERTIFICAT_REQUEST=$CA_DIR/RootCerificatRequest.csr
CA_CERTIFICAT=$CA_DIR/RootCerificat.crt


CA_COUNTRY=US
CA_STATE=Utah
CA_LOCALITY=Lehi
CA_ORGANIZATION_NAME=CertificateAuthority
CA_ORGANIZATION_UNIT_NAME=IT
CA_COMMON_NAME=www.ca.com
CA_EMAIL=
CA_PASSWORD=



echo " "
echo "## Make Directories ...##"

if [ -d $SERVER_DIR ]; then
	rm -d $SERVER_DIR
fi

if [ -d $DEVICE_DIR ]; then
	rm -d $DEVICE_DIR
fi

if [ -d $CA_DIR ]; then
	rm -d $CA_DIR
fi

if [ ! -d $SERVER_DIR ]; then
	mkdir $SERVER_DIR
fi

if [ ! -d $DEVICE_DIR ]; then
	mkdir $DEVICE_DIR
fi

if [ ! -d $CA_DIR ]; then
	mkdir $CA_DIR
fi


echo " "
echo "## CertificateAuthority ##"
if [ ! -f $CA_PRIVATE_KEY ]; then
	echo "Create the root key"
	openssl ecparam -out $CA_PRIVATE_KEY -name prime256v1 -genkey
	
	echo "Create a Root Certificate and self-sign it"
	SUGJECT_ARG="/C=$CA_COUNTRY/ST=$CA_STATE/L=$CA_LOCALITY/O=$CA_ORGANIZATION_NAME/OU=$CA_ORGANIZATION_UNIT_NAME/CN=$CA_COMMON_NAME"
	openssl req -new -sha256 -key $CA_PRIVATE_KEY -out $CA_CERTIFICAT_REQUEST -subj $SUGJECT_ARG
	openssl x509 -req -sha256 -days 365 -in $CA_CERTIFICAT_REQUEST -signkey $CA_PRIVATE_KEY -out $CA_CERTIFICAT
fi

echo " "
echo "## Server ##"

if [ ! -f $SERVER_PLAIN_DIR ]; then
	echo "## Create Message"
	mkdir $SERVER_PLAIN_DIR
	echo  "Portez ce vieux whisky au juge blond qui fume." > $SERVER_PLAIN_FILE 
	echo  "The quick brown fox jumps over the lazy dog." >> $SERVER_PLAIN_FILE 
	echo "## Create Large Text File"
	yes "Some text" | head -n 100 > $SERVER_PLAIN_DIR/1ko-file.txt
fi

echo "# Server generates a private key"
openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 -pkeyopt rsa_keygen_pubexp:3 -out $SERVER_PRIVATE_KEY

echo "# Server generates a public key"
openssl pkey -in $SERVER_PRIVATE_KEY -pubout -out $SERVER_PUBLIC_KEY

echo "# Server generates a certificate signing request"
SUGJECT_ARG="/C=$SERVER_COUNTRY/ST=$SERVER_STATE/L=$SERVER_LOCALITY/O=$SERVER_ORGANIZATION_NAME/OU=$SERVER_ORGANIZATION_UNIT_NAME/CN=$SERVER_COMMON_NAME"
openssl req -new -key $SERVER_PRIVATE_KEY -out $SERVER_CERTIFICAT_REQUEST -subj $SUGJECT_ARG

echo " "
echo "## Device ##"

echo "# Device generates a private key"
openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 -pkeyopt rsa_keygen_pubexp:3 -out $DEVICE_PRIVATE_KEY

echo "# Device generates a public key"
openssl pkey -in $DEVICE_PRIVATE_KEY -pubout -out $DEVICE_PUBLIC_KEY

echo "# Device generates a certificate signing request"
SUGJECT_ARG="/C=$DEVICE_COUNTRY/ST=$DEVICE_STATE/L=$DEVICE_LOCALITY/O=$DEVICE_ORGANIZATION_NAME/OU=$DEVICE_ORGANIZATION_UNIT_NAME/CN=$DEVICE_COMMON_NAME"
openssl req -new -key $DEVICE_PRIVATE_KEY -out $DEVICE_CERTIFICAT_REQUEST -subj $SUGJECT_ARG



echo " "
echo "## CertificateAuthority ##"
echo " "

echo "# CA generates and signs a certificate for Server"
openssl x509 -req -in $SERVER_CERTIFICAT_REQUEST -CA $CA_CERTIFICAT -CAkey $CA_PRIVATE_KEY -CAcreateserial -out $SERVER_CERTIFICAT -days 500 -sha256

echo "# CA generates and signs a certificate for Device"
openssl x509 -req -in $DEVICE_CERTIFICAT_REQUEST -CA $CA_CERTIFICAT -CAkey $CA_PRIVATE_KEY -CAcreateserial -out $DEVICE_CERTIFICAT -days 500 -sha256


if test -f "$SERVER_CERTIFICAT"; then
	echo "# Server send to Device his Server Certificat"
	cp $SERVER_CERTIFICAT $DEVICE_DIR
fi

if test -f "$DEVICE_CERTIFICAT"; then
	echo "# Device send to Server his Device Certificat"
	cp $DEVICE_CERTIFICAT $SERVER_DIR
fi



if test -f "$SERVER_DEVICE_CERTIFICAT"; then
	echo "# Server verifies Device’s public certificate"
	openssl verify -CAfile $CA_CERTIFICAT $SERVER_DEVICE_CERTIFICAT
	
	echo "# Server extracts Device’s public key"
	openssl x509 -pubkey -in $SERVER_DEVICE_CERTIFICAT -noout > $SERVER_DEVICE_PUBLIC_KEY
	
	
	echo "# Server tries to encrypt her largefile.txt with Device’s public key"
	openssl pkeyutl -encrypt -in $SERVER_PLAIN_FILE -pubin -inkey $SERVER_DEVICE_PUBLIC_KEY -out $SERVER_ENCRYPTED_FILE
	
	echo "# Server generates a symmetric key"
	openssl rand -base64 32 -out $SERVER_SYMMETRIC_KEY
	
	if test -f "$SERVER_SYMMETRIC_KEY"; then
		echo "# Server encrypts Unique Symmetric using Device’s public key"
		openssl pkeyutl -encrypt -in $SERVER_SYMMETRIC_KEY -pubin -inkey $SERVER_DEVICE_PUBLIC_KEY -out $SERVER_ENCRYPTED_SYMMETRIC_KEY
	
		echo "# Server hashes Unique Symmetric and encrypts it using her private key"
		#openssl dgst -sha1 -sign privkey-A.pem -out signature.bin symkey.pe
		openssl dgst -sha1 -sign $SERVER_PRIVATE_KEY -out $SERVER_SYMMETRIC_KEY_SIGNATURE $SERVER_SYMMETRIC_KEY
		
		echo "# Server encrypts her largefile.txt with the symmetric key"
		openssl enc -aes-256-cbc -pass file:$SERVER_SYMMETRIC_KEY -p -md sha256 -in $SERVER_PLAIN_FILE -out $SERVER_ENCRYPTED_FILE
	fi

	if test -f "$SERVER_ENCRYPTED_SYMMETRIC_KEY"; then
		echo "# Server send to Device the Encrypted Symmetric key"
		cp $SERVER_ENCRYPTED_SYMMETRIC_KEY $DEVICE_DIR
	fi
	
	if test -f "$SERVER_SYMMETRIC_KEY_SIGNATURE"; then
		echo "# Server send to Device the Encrypted Symmetric Signature"
		cp $SERVER_SYMMETRIC_KEY_SIGNATURE $DEVICE_DIR
	fi
	
	if test -f "$SERVER_ENCRYPTED_FILE"; then
		echo "# Server send to Device the Encrypted Message"
		cp $SERVER_ENCRYPTED_FILE $DEVICE_DIR
	fi

else
	echo "The $SERVER_CERTIFICAT file is missing"
fi


echo "## Device ##"
if test -f "$DEVICE_SERVER_DEVICE_CERTIFICAT"; then
	echo "# Device extracts Server’s public key"
	openssl x509 -pubkey -in $DEVICE_SERVER_DEVICE_CERTIFICAT -noout > $DEVICE_SERVER_PUBLIC_KEY
fi

if test -f "$DEVICE_ENCRYPTED_SYMMETRIC_KEY"; then
	echo "# Device decrypts Unique Symmetric using his private key"
	openssl pkeyutl -decrypt -in $DEVICE_ENCRYPTED_SYMMETRIC_KEY -inkey $DEVICE_PRIVATE_KEY -out $DEVICE_SYMMETRIC_KEY
fi

if test -f "$DEVICE_SYMMETRIC_KEY_SIGNATURE"; then
	echo "# Device verifies the message is from Server"
	openssl dgst -sha1 -verify $DEVICE_SERVER_PUBLIC_KEY -signature $DEVICE_SYMMETRIC_KEY_SIGNATURE $DEVICE_SYMMETRIC_KEY
	#openssl dgst -sha1 -verify $SERVER_PUBLIC_KEY -signature $DEVICE_SYMMETRIC_KEY_SIGNATURE $DEVICE_SYMMETRIC_KEY
fi

if test -f "$DEVICE_ENCRYPTED_FILE"; then
	echo "# Device decrypts ciphertext.bin with the same symmetric key"
	openssl enc -aes-256-cbc -d -pass file:$DEVICE_SYMMETRIC_KEY -p -md sha256 -in $DEVICE_ENCRYPTED_FILE -out $DEVICE_PLAIN_FILE
fi


#Verifying Your Keys Match
#openssl pkey -pubout -in .\private.key | openssl sha256
#openssl req -pubkey -in .\request.csr -noout | openssl sha256
#openssl x509 -pubkey -in .\certificate.crt -noout | openssl sha256

#Converting Certificate PEM to PKCS#12 Formats 
#openssl pkcs12 -export -name "yourdomain-digicert-(expiration date)" -out yourdomain.pfx -inkey yourdomain.key -in yourdomain.crt

#Converting Certificate  PKCS#12 to PEM Formats 
#openssl pkcs12 -in yourdomain.pfx -nocerts -out yourdomain.key -nodes
#openssl pkcs12 -in yourdomain.pfx -nokeys -clcerts -out yourdomain.crt

# PEM to DER
#openssl x509 -inform PEM -in yourdomain.crt -outform DER -out yourdomain.der
#openssl rsa -inform PEM -in yourdomain.key -outform DER -out yourdomain_key.der

# DER to PEM
#openssl x509 -inform DER -in yourdomain.der -outform PEM -out yourdomain.crt
#openssl rsa -inform DER -in yourdomain_key.der -outform PEM -out yourdomain.key

echo " "
echo "# Create Shared Secret from Server Private key and device public key"
openssl pkeyutl -derive -inkey $SERVER_PRIVATE_KEY -peerkey $SERVER_DEVICE_PUBLIC_KEY -out $SERVER_DEVICE_PUBLIC_KEY.dat  
#certutil -hashfile $WORKSPACE_DIR/$ITEM/$KeyFileName.dat  SHA256 | cut -d ':' -f 1 > $WORKSPACE_DIR/$ITEM/$KeyFileName.sgn 
#xxd -r -p $WORKSPACE_DIR/$ITEM/$KeyFileName.sgn  $WORKSPACE_DIR/$ITEM/$KeyFileName.bin    

