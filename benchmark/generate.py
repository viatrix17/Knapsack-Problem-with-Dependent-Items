
import random
import sys

def ifCorrectArgs():
  N = len(sys.argv)
  if N < 3:
    print("Too few arguments",file=sys.stderr)
    sys.exit(1)
  elif N > 3:
    print("Too many arguments",file=sys.stderr)
    sys.exit(1)
  if int(sys.argv[1]) <= 0 or int(sys.argv[2]) < 0:
    print("Wrong arguments",file=sys.stderr)
    sys.exit(1)


def generate(n,k):
  "Model G(n,p)"
  edges = [] #list of tuples
  for i in range(k):
    a = random.randint(1,n)
    b = random.randint(1,n)
    while (a,b) in edges or a == b:
       a = random.randint(1,n)
       b = random.randint(1,n)
    edges.append((a,b))

  return edges
 
def write_to_file(f,n,type,capacity):
    items = []
    sum = 0
    for i in range(n): #numer waga i wartość
        w = random.randint(1,100)
        v = random.randint(1,100)
        sum += w
        items.append((w,v))
    k = random.randint(0,n)

    edges = generate(n,k)
    if type == 2:
       f.write(f"{bf} {random.randint(int(sum/2),sum)}\n")
    else:
       f.write(f"{n} {capacity}\n")

    i = 1
    for item in items:
       f.write(f"{i} {item[0]} {item[1]}\n")
       i += 1
    f.write(f"{k}\n")
    for edge in edges:
        f.write(f"{edge[0]} {edge[1]}\n")

if __name__ == "__main__": 
  #ifCorrectArgs() 

  capacities = [1000, 1500, 2000, 2500, 3000]
  number_of_items = [100, 150, 200, 250, 300, 350, 400, 450]
  N = 100
 
  for capacity in capacities:
    print("capacity = ", capacity)
    if capacity == 2500:
      for n in number_of_items:
        print("number = ", n)
        name = "./data/diffNumber/items" + str(n)
        f = open(name, "w")
        write_to_file(f,n,0,capacity)
        f.close()
    name = "./data/diffCapacity/capacity" + str(capacity)
    f = open(name, "w")
    write_to_file(f,N,1,capacity)
    f.close()

  for bf in range(3,14):
      if bf < 10:
        name = "./data/bruteForce/brute0" + str(bf)
      else:
         name = "./data/bruteForce/brute" + str(bf)
      f = open(name, "w")
      write_to_file(f,bf,2,0)
      f.close()
  
  
