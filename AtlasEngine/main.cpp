#include "pch.h"

struct Global global;

int main()
{
    printf("Globals: %d bytes\n", sizeof(global));

    // this should be the one and only "malloc"
    void* mempackRamData = malloc(ramSize);

    // initialize mempack for RAM
    MEMPACK_Init(&global.ramPack, mempackRamData, ramSize, "ram");

    // create a window, create context,
    // create GPU Mempacks, etc
    InitOpenGL();

    // Load all sprites
    Sprite* manSprite = Sprite_Init("Assets/man.bmp");
    Sprite* coinSprite = Sprite_Init("Assets/coin.bmp");

    // Rather than allocating directly to the stack allocator, we make a buffer
    // with slots, so we can add and remove things as we need, without needing bookmarks.
    // Then, the stack allocator can continue allocating while we add/remove members
    Pool_Init(&global.pool[PL_THREAD],   10, sizeof(Thread),   "ThreadPool");
    Pool_Init(&global.pool[PL_INSTANCE], 10, sizeof(Instance), "InstancePool");

    // Initialize Instance in Instance Pool
    Instance* manInst1 = (Instance*)Pool_NewItem(&global.pool[PL_INSTANCE]);
    manInst1->sprite = manSprite;

    // draw person on top (small depth)
    manInst1->depth = 0.1f;
    manInst1->posX = -0.5f;
    manInst1->posY = -0.5f;
    manInst1->scaleX = 0.00075f;
    manInst1->scaleY = 0.00075f;

    // Initialize Thread, this Thread_Init pulls from Thread Pool
    Thread* manThread = Thread_Init(TH_PLAYER, Player_OnTick);

    // link instance and thread
    manThread->inst = manInst1;
    manInst1->thread = manThread;
    
    // spawn 5 coins
    for (int i = 0; i < 5; i++)
    {
        // Notice now we dont need an array,
        // because threads are linked, so we
        // can lose the pointers and still 
        // access the data later when we need it
        Instance* in = (Instance*)Pool_NewItem(&global.pool[PL_INSTANCE]);

        // coin doesn't need an update function,
        // but we will use it's onCollide pointer
        // in future tutorials
        Thread* th = Thread_Init(TH_COIN, nullptr);

        in->sprite = coinSprite;

        // link instance and thread
        th->inst = in;
        in->thread = th;

        // draw coins on bottom (larger depth)
        in->depth = 0.2f; 
        in->posX = 0.5f;
        in->posY = -0.5f + 0.2f * i;
        in->scaleX = 0.002f;
        in->scaleY = 0.002f;
    }

    // timer variables
    clock_t start;
    clock_t end;

    // initialize time
    start = clock();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(global.window))
    {
        // measure elapsed time per frame
        end = clock();
        global.elapsedTime = (end - start) / 1000.0f;
        start = clock();

        // Update controller "hold" variable
        glfwPollEvents();

        // Determine Tap and Release
        Input_ProcessState();

        // Loop through all threads in use
        Thread* th = (Thread*)global.pool[PL_THREAD].linkedList_TakenItems;
        while (th != nullptr)
        {
            if (th->funcOnTick != nullptr)
            {
                // execute thread's function,
                // passing itself as a parameter
                th->funcOnTick(th);
            }

            // go to next thread
            th = th->next;
        }

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Loop through all threads in use
        Instance* in = (Instance*)global.pool[PL_INSTANCE].linkedList_TakenItems;
        while (in != nullptr)
        {
            Instance_Draw(in);

            // go to next thread
            in = in->next;
        }

        // update swapchain
        glfwSwapBuffers(global.window);
    }

    // erase all allocated data
    MEMPACK_Clean(&global.ramPack);

    // erase resources
    delete mempackRamData;
    mempackRamData = nullptr;

    CleanOpenGL();

    // end program
    return 0;
}