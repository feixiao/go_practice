package main

import (
	"sync"

	"github.com/judwhite/go-svc"

	log "github.com/feixiao/infra/log"
)

// program implements svc.Service
type Program struct {
	wg   sync.WaitGroup
	quit chan struct{}

	isDec bool
	f     *FileEx
}

func (p *Program) Init(env svc.Environment) error {
	log.Info("is win service? %v\n", env.IsWindowsService())
	return nil
}

func NewProgram(file, md5 string, isDec bool) *Program {
	var p Program
	p.isDec = isDec
	p.f = NewFile(file, md5)
	return &p
}

func (p *Program) Start() error {
	// The Start method must not block, or Windows may assume your service failed
	// to start. Launch a Goroutine here to do something interesting/blocking.

	log.Info("Program start")
	p.quit = make(chan struct{})

	var err error

	log.Info("key : %s", string(p.f.key))
	if p.isDec {
		err = p.f.Dec("")
	} else {
		err = p.f.Enc("")
	}

	if err != nil {
		log.Error("%+v", err)
	}

	log.Info("Ctrl + C to quit")
	return err
}

func (p *Program) Stop() error {
	// The Stop method is invoked by stopping the Windows service, or by pressing Ctrl+C on the console.
	// This method may block, but it's a good idea to finish quickly or your process may be killed by
	// Windows during a shutdown/reboot. As a general rule you shouldn't rely on graceful shutdown.
	// err := p.s.Stop()
	// if err != nil {
	// 	_ = log.Error(err)
	// }

	log.Info("Stopping...")
	close(p.quit)
	p.wg.Wait()
	log.Info("Stopped.")
	return nil
}
