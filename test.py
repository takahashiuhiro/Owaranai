from torch import nn, Tensor
import torch.nn.functional as F
import torch
import math
from torch.autograd.variable import Variable
import typing
import random
import tqdm


    

q = torch.tensor(
         [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24.])
q = q.view([3,2,4])

q.requires_grad = True

g = torch.nn.LayerNorm([2,4])

asd = g(q)
print(asd)
r = asd.sum()
r.backward()

print(r)
print(q.grad)

