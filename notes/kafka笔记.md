# kafka笔记

## 1. topic

1. 查看topic：进入kafka的bin目录下`cd /opt/kafka/kafka_2.12-2.4.0/bin`，使用

   ```shell
   # 注意port是zookeeper的port，使用docker ps -a可以查看
   ./kafka-topics.sh --zookeeper ip:port --list
   ```

   

2. 创建topic：

   ```shell
   ./kafka-topics.sh --create --zookeeper ip:port --topic topic_name --partitions 1 --replication-factor 1
   ```

   

3. 待续