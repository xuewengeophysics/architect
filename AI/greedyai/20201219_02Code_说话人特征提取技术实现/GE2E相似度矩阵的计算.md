# GE2E相似度矩阵的计算

## 1. 相似度矩阵

### 1.1 符号的含义

+ **k：第k个speaker**；
+ **M：enrollment utterances的个数**；
+ **ji：第j个人的第i个utterances**；

![GE2E相似度矩阵的计算-f1.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/20201219_02Code_%E8%AF%B4%E8%AF%9D%E4%BA%BA%E7%89%B9%E5%BE%81%E6%8F%90%E5%8F%96%E6%8A%80%E6%9C%AF%E5%AE%9E%E7%8E%B0/figures/GE2E%E7%9B%B8%E4%BC%BC%E5%BA%A6%E7%9F%A9%E9%98%B5%E7%9A%84%E8%AE%A1%E7%AE%97-f1.png?raw=true)  



  ![GE2E相似度矩阵的计算-f2.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/20201219_02Code_%E8%AF%B4%E8%AF%9D%E4%BA%BA%E7%89%B9%E5%BE%81%E6%8F%90%E5%8F%96%E6%8A%80%E6%9C%AF%E5%AE%9E%E7%8E%B0/figures/GE2E%E7%9B%B8%E4%BC%BC%E5%BA%A6%E7%9F%A9%E9%98%B5%E7%9A%84%E8%AE%A1%E7%AE%97-f2.png?raw=true)  



![GE2E相似度矩阵的计算-f3.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/20201219_02Code_%E8%AF%B4%E8%AF%9D%E4%BA%BA%E7%89%B9%E5%BE%81%E6%8F%90%E5%8F%96%E6%8A%80%E6%9C%AF%E5%AE%9E%E7%8E%B0/figures/GE2E%E7%9B%B8%E4%BC%BC%E5%BA%A6%E7%9F%A9%E9%98%B5%E7%9A%84%E8%AE%A1%E7%AE%97-f3.png?raw=true)  



![GE2E相似度矩阵的计算-f4.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/20201219_02Code_%E8%AF%B4%E8%AF%9D%E4%BA%BA%E7%89%B9%E5%BE%81%E6%8F%90%E5%8F%96%E6%8A%80%E6%9C%AF%E5%AE%9E%E7%8E%B0/figures/GE2E%E7%9B%B8%E4%BC%BC%E5%BA%A6%E7%9F%A9%E9%98%B5%E7%9A%84%E8%AE%A1%E7%AE%97-f4.png?raw=true)   



![GE2E相似度矩阵的计算-f5.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/20201219_02Code_%E8%AF%B4%E8%AF%9D%E4%BA%BA%E7%89%B9%E5%BE%81%E6%8F%90%E5%8F%96%E6%8A%80%E6%9C%AF%E5%AE%9E%E7%8E%B0/figures/GE2E%E7%9B%B8%E4%BC%BC%E5%BA%A6%E7%9F%A9%E9%98%B5%E7%9A%84%E8%AE%A1%E7%AE%97-f5.png?raw=true)  

### 1.2 计算方法与代码的映射

![GE2E相似度矩阵的计算-f6.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/20201219_02Code_%E8%AF%B4%E8%AF%9D%E4%BA%BA%E7%89%B9%E5%BE%81%E6%8F%90%E5%8F%96%E6%8A%80%E6%9C%AF%E5%AE%9E%E7%8E%B0/figures/GE2E%E7%9B%B8%E4%BC%BC%E5%BA%A6%E7%9F%A9%E9%98%B5%E7%9A%84%E8%AE%A1%E7%AE%97-f6.png?raw=true)  

+ **公式中i的取值范围为range(utterances_per_speaker)，utterances_per_speaker也就是公式中的M**；
+ **-i表示排除该说话者的第i个声音**；
+ **公式中j的取值范围为range(speakers_per_batch)**；
+ **公式中k表示第k个speaker，它的取值范围为range(speakers_per_batch)**；
+ **相似度矩阵Sji,k的维度就是(speakers_per_batch, utterances_per_speaker, speakers_per_batch)**；

```python
#Real-Time-Voice-Cloning/encoder/model.py
	def similarity_matrix(self, embeds):
        """
        Computes the similarity m    def similarity_matrix(self, embeds):
        """
        Computes the similarity matrix according the section 2.1 of GE2E.
        ##每个人说过的每一段话的固定长度的embedding特征
        :param embeds: the embeddings as a tensor of shape (speakers_per_batch, 
        utterances_per_speaker, embedding_size)
        :return: the similarity matrix as a tensor of shape (speakers_per_batch,
        utterances_per_speaker, speakers_per_batch)
        """
        speakers_per_batch, utterances_per_speaker = embeds.shape[:2]

        ##求质心(包含自身，不包含自身)
        ##1.包含自身
        # Inclusive centroids (1 per speaker). Cloning is needed for reverse differentiation       ##针对每个说话者进行处理
        ##针对每个说话者，对该说话者所有的说话片段进行求平均
        centroids_incl = torch.mean(embeds, dim=1, keepdim=True)                                   ##(speakers_per_batch, 1, embedding_size)
        centroids_incl = centroids_incl.clone() / torch.norm(centroids_incl, dim=2, keepdim=True)  ##(speakers_per_batch, 1, embedding_size)

        ##2.不包含自身
        # Exclusive centroids (1 per utterance)                                                    ##针对每个说话者的每个声音的处理
        ##排除某个说话者的某一个声音，这个处理很巧妙，利用广播机制实现了对每1个声音的处理
        centroids_excl = (torch.sum(embeds, dim=1, keepdim=True) - embeds)                         ##(speakers_per_batch, utterances_per_speaker, embedding_size)
        centroids_excl /= (utterances_per_speaker - 1)
        centroids_excl = centroids_excl.clone() / torch.norm(centroids_excl, dim=2, keepdim=True)  ##(speakers_per_batch, utterances_per_speaker, embedding_size)

        # Similarity matrix. The cosine similarity of already 2-normed vectors is simply the dot
        # product of these vectors (which is just an element-wise multiplication reduced by a sum).
        # We vectorize the computation for efficiency.
        sim_matrix = torch.zeros(speakers_per_batch, utterances_per_speaker,
                                 speakers_per_batch).to(self.loss_device)
        ##对角线为1其它为0的矩阵，为了便于处理论文中的公式(9)
        mask_matrix = 1 - np.eye(speakers_per_batch, dtype=np.int)
        for j in range(speakers_per_batch):
            mask = np.where(mask_matrix[j])[0]
            sim_matrix[mask, :, j] = (embeds[mask] * centroids_incl[j]).sum(dim=2)                  ##第j个说话者与其它说话者的cosine
            sim_matrix[j, :, j] = (embeds[j] * centroids_excl[j]).sum(dim=1)
        
        ## Even more vectorized version (slower maybe because of transpose)
        # sim_matrix2 = torch.zeros(speakers_per_batch, speakers_per_batch, utterances_per_speaker
        #                           ).to(self.loss_device)
        # eye = np.eye(speakers_per_batch, dtype=np.int)
        # mask = np.where(1 - eye)
        # sim_matrix2[mask] = (embeds[mask[0]] * centroids_incl[mask[1]]).sum(dim=2)
        # mask = np.where(eye)
        # sim_matrix2[mask] = (embeds * centroids_excl).sum(dim=2)
        # sim_matrix2 = sim_matrix2.transpose(1, 2)
        
        sim_matrix = sim_matrix * self.similarity_weight + self.similarity_bias
        return sim_matrixatrix according the section 2.1 of GE2E.
        :param embeds: the embeddings as a tensor of shape (speakers_per_batch, 
        utterances_per_speaker, embedding_size)
        :return: the similarity matrix as a tensor of shape (speakers_per_batch,
        utterances_per_speaker, speakers_per_batch)
        """
        speakers_per_batch, utterances_per_speaker = embeds.shape[:2]
        
        # Inclusive centroids (1 per speaker). Cloning is needed for reverse differentiation
        centroids_incl = torch.mean(embeds, dim=1, keepdim=True) # (speakers_per_batch, 1, embedding_size)
        centroids_incl = centroids_incl.clone() / torch.norm(centroids_incl, dim=2, keepdim=True)

        # Exclusive centroids (1 per utterance)
        centroids_excl = (torch.sum(embeds, dim=1, keepdim=True) - embeds)
        centroids_excl /= (utterances_per_speaker - 1)
        centroids_excl = centroids_excl.clone() / torch.norm(centroids_excl, dim=2, keepdim=True)

        # Similarity matrix. The cosine similarity of already 2-normed vectors is simply the dot
        # product of these vectors (which is just an element-wise multiplication reduced by a sum).
        # We vectorize the computation for efficiency.
        sim_matrix = torch.zeros(speakers_per_batch, utterances_per_speaker,
                                 speakers_per_batch).to(self.loss_device)
        mask_matrix = 1 - np.eye(speakers_per_batch, dtype=np.int)
        for j in range(speakers_per_batch):
            mask = np.where(mask_matrix[j])[0]
            sim_matrix[mask, :, j] = (embeds[mask] * centroids_incl[j]).sum(dim=2)
            sim_matrix[j, :, j] = (embeds[j] * centroids_excl[j]).sum(dim=1)
        
        ## Even more vectorized version (slower maybe because of transpose)
        # sim_matrix2 = torch.zeros(speakers_per_batch, speakers_per_batch, utterances_per_speaker
        #                           ).to(self.loss_device)
        # eye = np.eye(speakers_per_batch, dtype=np.int)
        # mask = np.where(1 - eye)
        # sim_matrix2[mask] = (embeds[mask[0]] * centroids_incl[mask[1]]).sum(dim=2)
        # mask = np.where(eye)
        # sim_matrix2[mask] = (embeds * centroids_excl).sum(dim=2)
        # sim_matrix2 = sim_matrix2.transpose(1, 2)
        
        sim_matrix = sim_matrix * self.similarity_weight + self.similarity_bias
        return sim_matrix
```

