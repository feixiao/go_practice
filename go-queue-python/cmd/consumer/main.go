// Go 消费者：使用 go-zero go-queue (kq) 从 Kafka 消费消息
// 可接收来自 Python 生产者发送的消息
package main

import (
	"context"
	"encoding/json"
	"fmt"
	"os"
	"os/signal"
	"syscall"

	"github.com/zeromicro/go-queue/kq"
)

type Message struct {
	Source    string `json:"source"`
	Content   string `json:"content"`
	Timestamp string `json:"timestamp"`
}

func main() {
	// Kafka 配置（与 config.yaml 保持一致，也可通过环境变量覆盖）
	c := kq.KqConf{
		Brokers:    []string{"127.0.0.1:9092"},
		Group:      "go-python-group",
		Topic:      "go-python-topic",
		Offset:     "first",
		Consumers:  2,
		Processors: 4,
	}
	c.Name = "go-python-consumer"

	// 创建消费者队列
	q := kq.MustNewQueue(c, kq.WithHandle(func(ctx context.Context, key, value string) error {
		var msg Message
		if err := json.Unmarshal([]byte(value), &msg); err != nil {
			// 非 JSON 格式也打印
			fmt.Printf("[Go Consumer] 收到: key=%s value=%s\n", key, value)
			return nil
		}
		fmt.Printf("[Go Consumer] 收到来自 %s: %s (时间: %s)\n", msg.Source, msg.Content, msg.Timestamp)
		return nil
	}))
	defer q.Stop()

	// 处理 Ctrl+C
	sigChan := make(chan os.Signal, 1)
	signal.Notify(sigChan, syscall.SIGINT, syscall.SIGTERM)
	go func() {
		<-sigChan
		q.Stop()
		os.Exit(0)
	}()

	fmt.Println("Go Consumer 已启动，等待消息 (Ctrl+C 退出)")
	fmt.Println("Topic:", c.Topic)
	fmt.Println("---")

	q.Start()
}
