# GE2E相似度矩阵的计算

## 1. 相似度矩阵

### 1.1 符号的含义

+ **k：第k个speaker**；
+ **M：enrollment utterances的个数**；
+ **ji：第j个人的第i个utterances**；

![image-20201215213958878](C:\Users\wenxue\AppData\Roaming\Typora\typora-user-images\image-20201215213958878.png)  



![image-20201215214025544](C:\Users\wenxue\AppData\Roaming\Typora\typora-user-images\image-20201215214025544.png)  



![image-20201215214256120](C:\Users\wenxue\AppData\Roaming\Typora\typora-user-images\image-20201215214256120.png)  

  

![image-20201215214435539](C:\Users\wenxue\AppData\Roaming\Typora\typora-user-images\image-20201215214435539.png)  



![image-20201215215831266](C:\Users\wenxue\AppData\Roaming\Typora\typora-user-images\image-20201215215831266.png)  



![image-20201215215927232](C:\Users\wenxue\AppData\Roaming\Typora\typora-user-images\image-20201215215927232.png)  



+ **公式中j的取值范围为range(speakers_per_batch)**；
+ **公式中i的取值范围为range(utterances_per_speaker)，utterances_per_speaker也就是公式中的M**；
+ **公式中k表示第k个speaker，它的取值范围为range(speakers_per_batch)**；
+ **相似度矩阵Sji,k的维度就是(speakers_per_batch, utterances_per_speaker, speakers_per_batch)**；

```python
    def similarity_matrix(self, embeds):
        """
        Computes the similarity matrix according the section 2.1 of GE2E.
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

