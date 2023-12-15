package main

import (
	"fmt"
	"time"

	log "github.com/feixiao/infra/log"
	"github.com/judwhite/go-svc"
	"github.com/mkideal/cli"
)

type argT struct {
	// Version bool `cli:"v,version" usage:"show the version of roomserver"`
	Help bool `cli:"h,help" usage:"show help"`

	File string `cli:"*f,file" usage:"需要被加密的文件"`
	MD5  string `cli:"m,md5" usage:"加密使用的MD5值" dft:"OGU1NTlmZDA2MmJmMWI5MGYwMjBmOTUzMTBiYWEy"`
	Dec  bool   `cli:"e,enc" usage:"指示加密或者解密"`
}

func (argv *argT) AutoHelp() bool {
	return argv.Help
}

// 加密： ./enc -f="../data/test.txt"
// 解密： ./enc -f="../data/test.txt.bin" -e=true
func main() {
	args := &argT{}

	orig := "hello world hello world hello world hello world hello world hello world hello world hello world hello world"
	key := "123456781234567812345678"
	fmt.Println("原文：", orig)

	encryptCode := AesEncryptPkcs7(orig, key)
	fmt.Println("密文：", encryptCode)

	decryptCode := AesDecryptPkcs7(encryptCode, key)
	fmt.Println("解密结果：", decryptCode)

	log.Info("args : %+v", *args)
	cli.Run(args, func(ctx *cli.Context) error {
		argv := ctx.Argv().(*argT)
		ctx.String("argv : %v!\n", argv)
		return nil
	})

	// 获取到参数
	log.Info("args : %+v", *args)

	prg := NewProgram(args.File, args.MD5, args.Dec)
	// Call svc.Run to start your program/service.
	if err := svc.Run(prg); err != nil {
		log.Error("%+v", err)
	}

	time.Sleep(2 * time.Second)

}
