// Go 生产者：使用 go-zero go-queue (kq) 发送消息到 Kafka
// Python 消费者可订阅同一 topic 接收这些消息
package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/zeromicro/go-queue/kq"
)

type Message struct {
	Source    string    `json:"source"`
	Content   string    `json:"content"`
	Timestamp time.Time `json:"timestamp"`
}

func main() {
	brokers := []string{"127.0.0.1:9092"}
	topic := "go-python-topic"

	// 使用 WithSyncPush 确保消息同步发送，便于演示
	pusher := kq.NewPusher(brokers, topic, kq.WithSyncPush())
	defer pusher.Close()

	ctx := context.Background()

	// 处理 Ctrl+C 优雅退出
	sigChan := make(chan os.Signal, 1)
	signal.Notify(sigChan, syscall.SIGINT, syscall.SIGTERM)

	fmt.Println("Go Producer 已启动，向 Kafka 发送消息 (Ctrl+C 退出)")
	fmt.Println("Topic:", topic)
	fmt.Println("---")

	count := 0
	ticker := time.NewTicker(2 * time.Second)
	defer ticker.Stop()

	for {
		select {
		case <-sigChan:
			fmt.Println("\n生产者已停止")
			return
		case <-ticker.C:
			count++
			msg := Message{
				Source:    "go",
				Content:   fmt.Sprintf("Hello from Go #%d", count),
				Timestamp: time.Now(),
			}
			body, err := json.Marshal(msg)
			if err != nil {
				log.Fatal(err)
			}

			if err := pusher.Push(ctx, string(body)); err != nil {
				log.Printf("发送失败: %v", err)
				continue
			}
			fmt.Printf("[Go] 已发送: %s\n", string(body))
		}
	}
}
