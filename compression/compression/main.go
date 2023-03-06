package main

import (
	"time"

	"github.com/feixiao/infra/log"
	"github.com/judwhite/go-svc"
	"github.com/mkideal/cli"
)

type argT struct {
	// Version bool `cli:"v,version" usage:"show the version of roomserver"`
	Help bool `cli:"h,help" usage:"show help"`

	File         string `cli:"*f,file" usage:"需要被压缩或者解压的文件"`
	Comporession bool   `cli:"c" usage:"指示压缩还是解压"`
}

func (argv *argT) AutoHelp() bool {
	return argv.Help
}

// ./compression -f ../data/test.txt -c=true
// ./compression -f ../data/test.txt.cmpr

// 标准工具： gzip -d ../data/test.txt.gz
func main() {
	args := &argT{}

	log.Info("args : %+v", *args)
	cli.Run(args, func(ctx *cli.Context) error {
		argv := ctx.Argv().(*argT)
		ctx.String("argv : %v!\n", argv)
		return nil
	})

	// 获取到参数
	log.Info("args : %+v", *args)

	prg := NewProgram(args.File, args.Comporession)
	// Call svc.Run to start your program/service.
	if err := svc.Run(prg); err != nil {
		log.Error("%+v", err)
	}

	time.Sleep(500 * time.Millisecond)

}
