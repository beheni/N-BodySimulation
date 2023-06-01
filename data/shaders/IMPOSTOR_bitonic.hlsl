#define BLOCKSIZE 1024

int Step;
int Length;

RWStructuredBuffer<int2> Data;

groupshared int2 shared_data[BLOCKSIZE * 2];

void LoadInput(int i, int j)
{
    shared_data[j] = (i < Length) ? Data[i] : 0x7FFFFFFF; //load input, if outside the range use some padding
}

void SaveOutput(int i, int j)
{
    if (i < Length)
        Data[i] = shared_data[j];
}

void LocalSort(uint GI, uint j, uint k)
{
    //bitonic sort magic
    uint mask = (k == j) ? (2 * k - 1) : k;
    uint e = GI % k + 2 * k * (GI / k);

    //get local pair of elements
    int2 id = int2(e, e ^ mask);
    int2 A = shared_data[id.x];
    int2 B = shared_data[id.y];

    //swap if unsorted
    if (A.x > B.x)
    {
        shared_data[id.x] = B;
        shared_data[id.y] = A;
    }
}

#pragma kernel BlockSort
[numthreads(BLOCKSIZE / 2, 1, 1)]
void BlockSort(int3 Gid : SV_GroupID, int3 DTid : SV_DispatchThreadID, int3 GTid : SV_GroupThreadID, int GI : SV_GroupIndex)
{
    int GID = Gid.x;

    if ((GID - 1) * BLOCKSIZE >= Length)
        return;

    //load a block
    LoadInput(GID * BLOCKSIZE + GI, GI);
    LoadInput(GID * BLOCKSIZE + GI + BLOCKSIZE / 2, GI + BLOCKSIZE / 2);

    GroupMemoryBarrierWithGroupSync();
    
    //sort the block using full bitonic sort
    [unroll]
    for (uint j = 1; j < BLOCKSIZE; j <<= 1)
        [unroll] 
        for (uint k = j; k > 0; k >>= 1)
        {
            LocalSort(GI, j, k);
            GroupMemoryBarrierWithGroupSync();
        }

    //save block into global memory
    SaveOutput(GID * BLOCKSIZE + GI, GI);
    SaveOutput(GID * BLOCKSIZE + GI + BLOCKSIZE / 2, GI + BLOCKSIZE / 2);
}

//get the id'th unique bitonic sorting pair
int2 getSortPair(int id, int step)
{
    float j = floor(sqrt(float(2 * step) + 1.0) - 0.5); //Major stage
    float n = round(float(step) - 0.5 * j * (j + 1.0)); //Minor stage
    uint B = int(round(exp2(j - n))); //Block size / 2
    uint mask = (n < 0.5) ? (2 * B - 1) : B;
    //first pair element
    //basically insert a 0 bit in the (log2(block_size) - 1) bit place
    uint e1 = id % B + 2 * B * (id / B);
    //partner
    uint e2 = e1 ^ mask;
    return int2(e1, e2);
}

#pragma kernel GlobalSort
[numthreads(BLOCKSIZE, 1, 1)]
void GlobalSort(int3 Gid : SV_GroupID, int3 DTid : SV_DispatchThreadID, int3 GTid : SV_GroupThreadID, int GI : SV_GroupIndex)
{
    //block ids
    int2 GID = getSortPair(Gid.x, Step);

    if ((GID.y - 1) * BLOCKSIZE >= Length)
        return;
    
    //load a pair of blocks
    LoadInput(GID.x * BLOCKSIZE + GI, GI);
    LoadInput(GID.y * BLOCKSIZE + GI, GI + BLOCKSIZE);
    
    GroupMemoryBarrierWithGroupSync();

    //bitonic mergesort of the blocks
    [unroll] 
    for (uint k = BLOCKSIZE; k > 0; k >>= 1)
    {
        LocalSort(GI, BLOCKSIZE, k);
        GroupMemoryBarrierWithGroupSync();
    }

    //save output
    SaveOutput(GID.x * BLOCKSIZE + GI, GI);
    SaveOutput(GID.y * BLOCKSIZE + GI, GI + BLOCKSIZE);
}