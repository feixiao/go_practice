package main

import (
	"io/ioutil"
	"sync"

	"github.com/judwhite/go-svc"
	"github.com/pkg/errors"

	log "github.com/feixiao/infra/log"
)

// program implements svc.Service
type Program struct {
	wg   sync.WaitGroup
	quit chan struct{}

	comporession bool
	file         string
}

func (p *Program) Init(env svc.Environment) error {
	log.Info("is win service? %v\n", env.IsWindowsService())
	return nil
}

func NewProgram(file string, comporession bool) *Program {
	var p Program
	p.comporession = comporession
	p.file = file
	return &p
}

func (p *Program) Start() error {
	// The Start method must not block, or Windows may assume your service failed
	// to start. Launch a Goroutine here to do something interesting/blocking.

	log.Info("Program start")
	p.quit = make(chan struct{})

	fileData, err := ioutil.ReadFile(p.file)
	if err != nil {
		return errors.WithMessagef(err, "ReadFile file:%s failed", p.file)
	}

	if p.comporession {
		cData, err := Comporession(fileData)
		if err != nil {
			return errors.WithMessage(err, "Comporess failed")
		}

		saveFile := p.file + ".cmpr"
		if err := ioutil.WriteFile(saveFile, *cData, 0644); err != nil {
			return errors.WithMessagef(err, "write file:%s failed", saveFile)
		}
	} else {

		// 进行解压
		uData, err := UnComporession(fileData)
		if err != nil {
			return errors.WithMessage(err, "UnComporess failed")
		}

		saveFile := p.file + ".ucmpr"
		if err := ioutil.WriteFile(saveFile, *uData, 0644); err != nil {
			return errors.WithMessagef(err, "write file:%s failed", saveFile)
		}

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
