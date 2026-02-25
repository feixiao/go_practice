#!/usr/bin/env python3
"""
Python 消费者：从 Kafka 消费消息
可接收来自 Go 生产者 (go-queue kq) 发送的消息
"""
import json
import signal
import sys

from kafka import KafkaConsumer
from kafka.errors import KafkaError

BROKERS = ["127.0.0.1:9092"]
TOPIC = "go-python-topic"
GROUP_ID = "python-consumer-group"


def main():
    consumer = KafkaConsumer(
        TOPIC,
        bootstrap_servers=BROKERS,
        group_id=GROUP_ID,
        auto_offset_reset="earliest",
        value_deserializer=lambda m: m.decode("utf-8"),
    )

    def shutdown(sig, frame):
        consumer.close()
        print("\n消费者已停止")
        sys.exit(0)

    signal.signal(signal.SIGINT, shutdown)
    signal.signal(signal.SIGTERM, shutdown)

    print("Python Consumer 已启动，等待消息 (Ctrl+C 退出)")
    print(f"Topic: {TOPIC}")
    print("---")

    try:
        for message in consumer:
            try:
                msg = json.loads(message.value)
                source = msg.get("source", "unknown")
                content = msg.get("content", message.value)
                ts = msg.get("timestamp", "")
                print(f"[Python Consumer] 收到来自 {source}: {content} (时间: {ts})")
            except json.JSONDecodeError:
                print(f"[Python Consumer] 收到: {message.value}")
    except KafkaError as e:
        print(f"Kafka 错误: {e}")
    finally:
        consumer.close()


if __name__ == "__main__":
    main()
