//
// Created by shousake on 2024/7/13.
//

#ifndef ROTATEDICK_BLOCKS_H
#define ROTATEDICK_BLOCKS_H
#include "functions.h"

SDL_Texture* QblockTexture;
SDL_Rect Qposition[4] =
{
{1,0,108,95},
{110,0,108,95},
{219,0,108,95},
{328,0,108,95}
};
SDL_Rect NullBlock = { 437,0,108,95 };
struct Questionblock
{
    int num;
    int count;
    int x;
    int y;
    int width;
    int height;
    int frequency;
    SDL_bool isUse;
    int BlockID;
    struct Questionblock* next;
};
struct NullBlock
{
    int x;
    int y;
    int width;
    int height;
    int BlockID;
    struct NullBlock* next;
};
struct NullBlock* Nblocks=NULL;
struct Questionblock* Qblocks=NULL;
SDL_bool initQblocks()
{

    QblockTexture = get_BMPTexture("pic/Qblocks.bmp");
    if (QblockTexture == NULL)
    {
        exit(1);
    }
    return SDL_TRUE;
}


SDL_bool putQblock() {
    struct Questionblock* current = Qblocks;
    while (current != NULL) {
        if (current->isUse) {
            SDL_Rect rec = {0,0,0,0};
            rec.x = current->x;
            rec.y = current->y;
            rec.w = current->width;
            rec.h = current->height;
            SDL_RenderCopyEx(renderer, QblockTexture, &Qposition[current->num], &rec, 0, NULL, SDL_FLIP_NONE);

            if ((current->count) == (current->frequency)) {
                current->num++;
                if (current->num == 4)
                    current->num = 0;
                current->count = 0;
            }
            else {
                current->count++;
            }
            
        }
        current = current->next; // �ƶ�����һ���ڵ�
    }

    return SDL_TRUE;
}
SDL_bool addQBlocks(int x, int y, int width, int height, int frequency, int BlockID)
{
    struct Questionblock* i;
    i = Qblocks;
    struct Questionblock* prev = NULL;

    while (i != NULL)
    {
        if (i->BlockID == BlockID)
        {
            return SDL_FALSE;
        }
        i = i->next;
    }
    i = Qblocks;
    while (i != NULL && i->isUse)
    {

        prev = i;
        i = i->next;
    }
    if (i != NULL)
    {
        i->x = x;
        i->y = y;
        i->width = width;
        i->height = height;
        i->frequency = frequency;
        i->BlockID = BlockID;
        i->count = 0;
        i->num = 0;
    }
    else {
        struct Questionblock* newBlock = (struct Questionblock*)malloc(sizeof(struct Questionblock)*2);
        if (newBlock == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return SDL_FALSE;
        }
        newBlock->x = x;
        newBlock->y = y;
        newBlock->width = width;
        newBlock->height = height;
        newBlock->isUse = SDL_TRUE;
        newBlock->frequency = frequency;
        newBlock->BlockID = BlockID;
        newBlock->next = NULL;
        newBlock->count = 0;
        newBlock->num = 0;
        if (prev != NULL) {
            prev->next = newBlock;
        }
        else {
            Qblocks = newBlock;  // �������Ϊ�գ�����ͷ�ڵ�
        }
    }

    return SDL_TRUE;
}
void freeQuestionblocks() {
    struct Questionblock* current = Qblocks;
    struct Questionblock* nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    Qblocks = NULL; // ȷ��ͷ�ڵ㱻����ΪNULL����ʾ���������
}
SDL_bool DeleteQblock(int BlockID) {
    struct Questionblock* current = Qblocks;
    struct Questionblock* prev = NULL;

    while (current != NULL) {
        if (current->BlockID == BlockID) {
            if (prev != NULL) {
                prev->next = current->next;
            }
            else {
                Qblocks = current->next; // ���ɾ������ͷ�ڵ�
            }
            free(current);
            return SDL_TRUE;
        }
        prev = current;
        current = current->next;
    }

    return SDL_FALSE; // δ�ҵ�ָ����BlockID
}
SDL_bool addNullBlock(int x, int y, int width, int height, int BlockID)
{
    struct NullBlock* i;
    i = Nblocks;
    struct NullBlock* prev = NULL;
    while (i != NULL)
    {
        if (i->BlockID == BlockID)
        {
            return SDL_FALSE;
        }
        prev = i;
        i = i->next;
    }
    // i = Nblocks;
    if (i != NULL)
    {
        i->x = x;
        i->y = y;
        i->width = width;
        i->height = height;

        i->BlockID = BlockID;
    }
    else {
        struct NullBlock* newBlock = (struct NullBlock*)malloc(sizeof(struct NullBlock));
        if (newBlock == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return SDL_FALSE;
        }
        newBlock->x = x;
        newBlock->y = y;
        newBlock->width = width;
        newBlock->height = height;


        newBlock->BlockID = BlockID;
        newBlock->next = NULL;
        if (prev != NULL) {
            prev->next = newBlock;
        }
        else {
            Nblocks = newBlock;  // �������Ϊ�գ�����ͷ�ڵ�
        }
    }

    return SDL_TRUE;

}
SDL_bool putNullBlocks()
{
    struct NullBlock* current = Nblocks;
    while (current != NULL) {

        SDL_Rect rec;
        rec.x = current->x;
        rec.y = current->y;
        rec.w = current->width;
        rec.h = current->height;
        SDL_RenderCopyEx(renderer, QblockTexture, &NullBlock, &rec, 0, NULL, SDL_FLIP_NONE);



        current = current->next; // �ƶ�����һ���ڵ�
    }

    return SDL_TRUE;
}
#endif //ROTATEDICK_BLOCKS_H

