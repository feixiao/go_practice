#!/bin/bash
# 验证 go-queue 与 Python 交互
set -e
cd "$(dirname "$0")"

echo "=== 1. 检查 Kafka ==="
docker compose ps | grep -q kafka || { echo "请先运行: docker compose up -d"; exit 1; }
echo "Kafka 已运行"

echo ""
echo "=== 2. 编译 Go 程序 ==="
go build -o /tmp/go-producer ./cmd/producer
go build -o /tmp/go-consumer ./cmd/consumer
echo "Go 编译成功"

echo ""
echo "=== 3. 测试 Python 环境 ==="
cd python && source .venv/bin/activate && python -c "import kafka; print('kafka-python OK')"
cd ..
echo "Python 依赖 OK"

echo ""
echo "=== 4. 启动验证（10秒）==="
echo "启动 Go 消费者..."
/tmp/go-consumer &
CONSUMER_PID=$!
sleep 2

echo "启动 Python 生产者，发送 2 条消息..."
cd python && source .venv/bin/activate && timeout 6 python producer.py 2>/dev/null || true
cd ..

sleep 2
kill $CONSUMER_PID 2>/dev/null || true
echo ""
echo "=== 验证完成 ==="
