package main

import (
	"bytes"
	"compress/gzip"
	"io/ioutil"
	"log"
)

func Comporession(data []byte) (*[]byte, error) {
	var buf bytes.Buffer
	w := gzip.NewWriter(&buf)
	if _, err := w.Write(data); err != nil {
		return nil, err
	}
	if err := w.Close(); err != nil {
		return nil, err
	}
	cData := buf.Bytes()

	return &cData, nil
}

func UnComporession(data []byte) (*[]byte, error) {
	b := bytes.NewReader(data)
	r, err := gzip.NewReader(b)
	if err != nil {
		return nil, err
	}

	uData, err := ioutil.ReadAll(r)
	if err != nil {
		log.Fatal(data)
		return nil, err
	}
	r.Close()

	return &uData, nil
}
