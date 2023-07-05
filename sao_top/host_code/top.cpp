#include <iostream>
#include <string>
#include "xcabac_top.h"
#include "top.h"

using namespace std;

#define DDR_BASE_ADDR               0x10000000
#define INP_BASE_ADDR               (DDR_BASE_ADDR + 0x05000000)
#define OUT_BASE_ADDR               (DDR_BASE_ADDR + 0x02000000)
#define ARITH_STATE_BASE_ADDR       (DDR_BASE_ADDR + 0x03000000)
#define CTX_BASE_ADDR               (DDR_BASE_ADDR + 0x04000000)
#define BST_BASE_ADDR               (DDR_BASE_ADDR + 0x01000000)
#define MAP_SIZE                    (getpagesize())
#define MAP_MASK                    (MAP_SIZE - 1)

int cabac_top(UChar globalCtx[MAX_NUM_CTX_MOD], arith_t initArithState[1], UChar bitStream[64], data_in_t data_in_s[1], data_out_t data_out_s[1]){
    // Initializing cabac IP
    XCabac_top cabac_ip;
    if(XCabac_top_Initialize(&cabac_ip, "cabac_top")){
        cout << "Failed to initialize Cabac IP" << endl;
        return 1;
    }

    int memfd;
    memfd = open("/dev/mem", O_RDWR | O_SYNC);
        if (memfd == -1) {
        printf("Can't open /dev/mem.\n");
        return 1;
    }

    volatile UChar *globalCtx_mapped;
    volatile arith_t *initArithState_mapped;
    volatile UChar *bitStream_mapped;
    volatile data_in_t *data_in_s_mapped;
    volatile data_out_t *data_out_s_mapped;

    void *mapped_bst, *mapped_inp, *mapped_out, *mapped_arith, *mapped_ctx;
    off_t base_bst = BST_BASE_ADDR;
    off_t base_inp = INP_BASE_ADDR;
    off_t base_out = OUT_BASE_ADDR;
    off_t base_arith  = ARITH_STATE_BASE_ADDR;
    off_t base_ctx = CTX_BASE_ADDR;

    //Mapping bitstream
    mapped_bst = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, base_bst & ~(MAP_MASK));
    if(mapped_bst == (void*) -1){
	    printf("Cant map the memory to user space\n");
	    return 2;
    }
    mapped_bst += (base_bst & MAP_MASK);
    bitStream_mapped = (volatile UChar * )mapped_bst;
    memcpy((UChar *)bitStream_mapped, bitStream, 64*sizeof(UChar));
    printf("Mapped bitstream to address %p\n", mapped_bst);
    
    //Mapping input
    mapped_inp = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, base_inp & ~(MAP_MASK));
    if(mapped_inp == (void*) -1){
	    printf("Cant map the memory to user space\n");
	    return 2;
    }
    mapped_inp += (base_inp & MAP_MASK);
    data_in_s_mapped = (volatile data_in_t * )mapped_inp;
    memcpy((data_in_t *)data_in_s_mapped, data_in_s, sizeof(data_in_t));
    printf("Mapped input to address %p\n", mapped_bst);
    
    //Mapping context
    mapped_ctx = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, base_ctx & ~(MAP_MASK));
    if(mapped_ctx == (void*) -1){
	    printf("Cant map the memory to user space\n");
	    return 2;
    }
    mapped_ctx += (base_ctx & MAP_MASK);
    globalCtx_mapped = (volatile UChar * )mapped_ctx;
    memcpy((UChar *)globalCtx_mapped, globalCtx, MAX_NUM_CTX_MOD*sizeof(UChar));
    printf("Mapped context to address %p\n", mapped_ctx);
    
    //Mapping arith state
    mapped_arith = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, base_arith & ~(MAP_MASK));
    if(mapped_arith == (void*) -1){
	    printf("Cant map the memory to user space\n");
	    return 2;
    }
    mapped_arith += (base_arith & MAP_MASK);
    initArithState_mapped = (volatile arith_t * )mapped_arith;
    memcpy((arith_t *)initArithState_mapped, initArithState, sizeof(arith_t));
    printf("Mapped arith state to address %p\n", mapped_arith);
    
    //Mapping out
    mapped_out = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, base_out & ~(MAP_MASK));
    if(mapped_out == (void*) -1){
	    printf("Cant map the memory to user space\n");
	    return 2;
    }
    mapped_out += (base_out & MAP_MASK);
    data_out_s_mapped = (volatile data_out_t * )mapped_out;
    printf("Mapped data_out to address %p\n", mapped_bst);
    
    XCabac_top_Set_gCtx(&cabac_ip, CTX_BASE_ADDR);
    XCabac_top_Set_bst(&cabac_ip, BST_BASE_ADDR);
    XCabac_top_Set_din(&cabac_ip, INP_BASE_ADDR);
    XCabac_top_Set_BaeState(&cabac_ip, ARITH_STATE_BASE_ADDR);
    XCabac_top_Set_gCtx_r(&cabac_ip, OUT_BASE_ADDR);


    XCabac_top_Start(&cabac_ip);

    while(!XCabac_top_IsDone(&cabac_ip)){
        cout << "Running ip" << endl;
    }

    memcpy(globalCtx, (UChar *)globalCtx_mapped, MAX_NUM_CTX_MOD*sizeof(UChar));
    memcpy(data_out_s, (data_out_t*)data_out_s_mapped, sizeof(data_out_t));
    memcpy(initArithState, (arith_t *)initArithState_mapped, sizeof(arith_t));
    
    XCabac_top_Release(&cabac_ip);

    if (munmap(mapped_inp, MAP_SIZE) == -1) {
        printf("Can't unmap memory from user space.\n");
        return 1;
    }
    if (munmap(mapped_out, MAP_SIZE) == -1) {
        printf("Can't unmap memory from user space.\n");
        return 1;
    }

    if (munmap(mapped_bst, MAP_SIZE) == -1) {
        printf("Can't unmap memory from user space.\n");
        return 1;
    }

    if (munmap(mapped_ctx, MAP_SIZE) == -1) {
        printf("Can't unmap memory from user space.\n");
        return 1;
    }
    if (munmap(mapped_arith, MAP_SIZE) == -1) {
        printf("Can't unmap memory from user space.\n");
        return 1;
    }


    close(memfd);
    return 0;
}




