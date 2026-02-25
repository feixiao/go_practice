#!/usr/bin/env python3
"""
Python 生产者：发送消息到 Kafka
Go 消费者 (go-queue kq) 可订阅同一 topic 接收这些消息
"""
import json
import signal
import sys
import time
from datetime import datetime

from kafka import KafkaProducer
from kafka.errors import KafkaError

BROKERS = ["127.0.0.1:9092"]
TOPIC = "go-python-topic"


def main():
    producer = KafkaProducer(
        bootstrap_servers=BROKERS,
        value_serializer=lambda v: json.dumps(v, default=str).encode("utf-8"),
    )

    def shutdown(sig, frame):
        producer.close()
        print("\n生产者已停止")
        sys.exit(0)

    signal.signal(signal.SIGINT, shutdown)
    signal.signal(signal.SIGTERM, shutdown)

    print("Python Producer 已启动，向 Kafka 发送消息 (Ctrl+C 退出)")
    print(f"Topic: {TOPIC}")
    print("---")

    count = 0
    try:
        while True:
            count += 1
            msg = {
                "source": "python",
                "content": f"Hello from Python #{count}",
                "timestamp": datetime.now().isoformat(),
            }
            producer.send(TOPIC, value=msg)
            producer.flush()
            print(f"[Python] 已发送: {json.dumps(msg, default=str)}")
            time.sleep(2)
    except KafkaError as e:
        print(f"Kafka 错误: {e}")
    finally:
        producer.close()


if __name__ == "__main__":
    main()
