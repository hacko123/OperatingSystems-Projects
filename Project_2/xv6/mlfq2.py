__author__ = 'Ashok Marannan'
import matplotlib.pyplot as plt
plt.xlabel("Time -->")
plt.ylabel("Priority Queue -->")
plt.plot([1,2,3,4,5,6,7,8,9,10,11], [0,1,1,2,2,2,2,3,3,3,3],marker="*",label="P1")
plt.plot([3,7,8,11,12,13,14,19], [0,1,1,2,2,2,2,3],marker="^",label="P2")
plt.plot([4,9,10,15,16,17,18,20,21,22], [0,1,1,2,2,2,2,3,3,3],marker="o",label="P3")
plt.legend()
#plt.axis([0, 6, 0, 20])
plt.xlim(0,30)
plt.yticks([0,1,2,3,4])
plt.savefig('mlfq_4')