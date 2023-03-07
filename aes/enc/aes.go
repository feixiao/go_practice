package main

import (
	"bytes"
	"crypto/aes"
	"crypto/cipher"
	"io/ioutil"
)

func EncryptFile(filepath string, savepath string, key []byte) error {

	fileData, err := ioutil.ReadFile(filepath)
	if err != nil {
		return err
	}

	data, err := AesEncrypt(fileData, key)
	if err != nil {
		return err
	}

	// // 进行压缩
	// cData, err := Comporession(data)
	// if err != nil {
	// 	return err
	// }

	if err := ioutil.WriteFile(savepath, data, 0644); err != nil {
		return err
	}
	return nil
}

func DecryptFile(filepath string, savepath string, key []byte) error {
	fileData, err := ioutil.ReadFile(filepath)
	if err != nil {
		return err
	}

	// // 进行解压
	// uData, err := UnComporession(fileData)
	// if err != nil {
	// 	log.Fatal(uData)
	// 	return err
	// }

	data, err := AesDecrypt(fileData, key)
	if err != nil {
		return err
	}

	if err := ioutil.WriteFile(savepath, []byte(data), 0644); err != nil {
		return err
	}

	return nil
}

func PKCS5Padding(ciphertext []byte, blockSize int) []byte {
	padding := blockSize - len(ciphertext)%blockSize
	padtext := bytes.Repeat([]byte{byte(padding)}, padding)
	return append(ciphertext, padtext...)
}

func PKCS5UnPadding(origData []byte) []byte {
	length := len(origData)
	unpadding := int(origData[length-1])
	return origData[:(length - unpadding)]
}

func AesEncrypt(origData, key []byte) ([]byte, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	blockSize := block.BlockSize()
	origData = PKCS5Padding(origData, blockSize)
	blockMode := cipher.NewCBCEncrypter(block, key[:blockSize])
	crypted := make([]byte, len(origData))
	blockMode.CryptBlocks(crypted, origData)
	return crypted, nil
}

func AesDecrypt(crypted, key []byte) ([]byte, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	blockSize := block.BlockSize()
	blockMode := cipher.NewCBCDecrypter(block, key[:blockSize])
	origData := make([]byte, len(crypted))
	blockMode.CryptBlocks(origData, crypted)
	origData = PKCS5UnPadding(origData)
	return origData, nil
}
