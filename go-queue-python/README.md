# go-zero go-queue 与 Python 消息队列交互

研究 go-zero 通过 go-queue (Kafka) 消息队列与 Python 程序的交互方式。

## 架构说明

```
┌─────────────────┐     Kafka      ┌─────────────────┐
│  Go Producer    │ ──────────────▶│  Python Consumer│
│  (go-queue kq)  │   go-python-   │  (kafka-python)  │
└─────────────────┘     topic       └─────────────────┘
        │                                    ▲
        │                                    │
        ▼                                    │
┌─────────────────┐     Kafka      ┌─────────────────┐
│  Python Producer│ ──────────────▶│  Go Consumer    │
│  (kafka-python) │   go-python-   │  (go-queue kq)  │
└─────────────────┘     topic       └─────────────────┘
```

**核心要点**：Go 和 Python 通过 **Kafka** 作为中间件通信。go-queue 的 `kq` 模块基于 `segmentio/kafka-go`，与 Python 的 `kafka-python` 使用相同的 Kafka 协议，因此可以无缝互通。

## 消息格式

统一使用 JSON，便于跨语言解析：

```json
{
  "source": "go" | "python",
  "content": "消息内容",
  "timestamp": "2025-02-25T10:00:00Z"
}
```

## 快速开始

### 1. 启动 Kafka

```bash
cd go-queue-python
docker-compose up -d
```

等待 Kafka 就绪（约 10-20 秒）。

### 2. 安装依赖

**Go：**
```bash
go mod tidy
# 若网络较慢，可尝试: GOPROXY=https://goproxy.cn,direct go mod tidy
```

**Python：**
```bash
cd python
python3 -m venv .venv
source .venv/bin/activate   # Windows: .venv\Scripts\activate
pip install -r requirements.txt
```

### 3. 运行示例

**场景 A：Go 发 → Python 收**

终端 1 启动 Go 生产者：
```bash
go run ./cmd/producer
```

终端 2 启动 Python 消费者：
```bash
cd python && source .venv/bin/activate && python consumer.py
```

**场景 B：Python 发 → Go 收**

终端 1 启动 Go 消费者：
```bash
go run ./cmd/consumer
```

终端 2 启动 Python 生产者：
```bash
cd python && source .venv/bin/activate && python producer.py
```

**场景 C：双向互通**

同时启动 Go 消费者、Python 消费者、Go 生产者、Python 生产者，可看到双方互相收发消息。

## 项目结构

```
go-queue-python/
├── cmd/
│   ├── producer/   # Go 生产者 (go-queue kq)
│   └── consumer/   # Go 消费者 (go-queue kq)
├── python/
│   ├── producer.py # Python 生产者
│   ├── consumer.py # Python 消费者
│   └── requirements.txt
├── config.yaml     # Go 消费者 Kafka 配置
├── docker-compose.yml
└── README.md
```

## 配置说明

| 配置项 | 说明 | 默认值 |
|--------|------|--------|
| Brokers | Kafka 地址 | 127.0.0.1:9092 |
| Topic | 主题名 | go-python-topic |
| Group | 消费者组 | go-python-group |
| Offset | 起始偏移 | first |

修改 `config.yaml` 或 Python 脚本中的 `BROKERS`、`TOPIC` 以适配你的环境。

## 技术要点

1. **协议兼容**：Kafka 是语言无关的，go-queue kq 与 kafka-python 都遵循 Kafka 协议。
2. **序列化**：建议使用 JSON 作为消息体，便于双方解析。
3. **消费者组**：Go 和 Python 可使用不同消费者组，实现多路消费。
4. **Topic**：双方必须订阅/发布同一 Topic 才能互通。

## 参考

- [go-queue](https://github.com/zeromicro/go-queue)
- [go-zero 消息队列文档](https://go-zero.dev/docs/tutorials/message-queue/kafka)
- [kafka-python](https://github.com/dpkp/kafka-python)
