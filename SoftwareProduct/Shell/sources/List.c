/** @addtogroup   LIST  
    @{
********************************************************************************
*   @name       LIST
*   @file       LIST.c
*
*   @brief      Implementation of a dynamic list.
*               List on HEAP. Implemented as array of pointers
*               to the effective data of the items.
*
*   @authors    A.Bader
*   @date       21.05.2014
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/
#include <string.h>
#include <stdlib.h> 

#include "List.h"

/*== Defines =================================================================*/
  #define MIN(x, y) (((x) < (y)) ? (x) : (y))
  
/*== Enumerations ============================================================*/

/*== Structures ==============================================================*/

/*== Global Variables ========================================================*/

/*== Module Constants ========================================================*/

/*== Function Prototypes =====================================================*/
  
/*== Module Variables ========================================================*/

/*== Weak Functions to uncouple Modules ======================================*/

/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
List_ts* List_Create(uint16_t unMaxEntries)
{
  uint32_t   ulLen;
  List_ts*  ptList;

  // allocat memory of list
  ulLen = sizeof(List_ts);
  ptList = malloc(ulLen);

  // allocat memory entries
  ulLen =  (sizeof(EntryList_ts)* unMaxEntries);
  ptList->Entries = malloc(ulLen);

  // init list
  ptList->MaxNumber = unMaxEntries;
  ptList->Numbers = 0;

  return ptList;
}

/*----------------------------------------------------------------------------*/
uint16_t List_ItemCount(const List_ts* ptList)
{
  return ptList->Numbers;
}

/*----------------------------------------------------------------------------*/
uint16_t List_AddItem(List_ts *ptList, const uint8_t *c_paucBuf, uint16_t Lenght)
{
  uint16_t   unIdx = List_ItemCount(ptList);

  if (unIdx >= ptList->MaxNumber)
    return 0;
  //ASSERT(c_paucBuf);
  ptList->Entries[unIdx].Entry = malloc(Lenght);
  memcpy(ptList->Entries[unIdx].Entry, c_paucBuf, Lenght);
  ptList->Entries[unIdx].Lenght = Lenght;

  unIdx++;
  ptList->Numbers = unIdx;

  return unIdx;
}

/*----------------------------------------------------------------------------*/
uint16_t List_DeleteItem(List_ts *ptList, uint16_t unIdx)
{
  uint8_t *pucItem = List_GetItem(ptList, unIdx, NULL);

  if (pucItem)
  {
    uint16_t  unCount = List_ItemCount(ptList);
    uint16_t  unToMove =  unCount - unIdx - 1;

  //  if (unCount <= unIdx)
  //    ASSERT_FAIL();  

    free(pucItem);
    memmove((uint8_t*)&ptList->Entries[unIdx], (uint8_t*)&ptList->Entries[unIdx+1], unToMove*sizeof(EntryList_ts));

    //ASSERT(ptList->Numbers);
    ptList->Numbers--;    
    return 0;
  }
  else
    return 1;
}

/*----------------------------------------------------------------------------*/
uint8_t* List_GetItem(const List_ts *ptList, uint16_t unIdx, uint16_t *pLenght)
{
  if (unIdx >= List_ItemCount(ptList))
    return 0;

  if (pLenght)
    *pLenght = ptList->Entries[unIdx].Lenght;

  return ptList->Entries[unIdx].Entry;
}

/*----------------------------------------------------------------------------*/
uint8_t* List_CopyItem(const List_ts *ptList, uint16_t unIdx, uint16_t *pLenght)
{
  uint8_t *paucBuf = NULL;
  
  if (unIdx >= List_ItemCount(ptList))
    return 0;

  paucBuf = malloc(ptList->Entries[unIdx].Lenght);
  memcpy(paucBuf, ptList->Entries[unIdx].Entry, ptList->Entries[unIdx].Lenght);

  if (pLenght)
    *pLenght = ptList->Entries[unIdx].Lenght;

  return paucBuf;
}


/*----------------------------------------------------------------------------*/
int List_SortCmpUp (const void * a, const void * b)
{
  EntryList_ts *pucA = (EntryList_ts *)a, *pucB = (EntryList_ts *)b;
  return ( memcmp((void *)pucA->Entry, (void *)pucB->Entry, MIN(pucA->Lenght, pucB->Lenght) ));
}

int List_SortCmpDown (const void * a, const void * b)
{
  EntryList_ts *pucA = (EntryList_ts *)a, *pucB = (EntryList_ts *)b;
  return ( memcmp((void *)pucB->Entry, (void *)pucA->Entry, MIN(pucA->Lenght, pucB->Lenght) ));
}

void List_Sort(List_ts *ptList, bool bUpDown)
{
  uint16_t unCount = List_ItemCount(ptList);
  qsort(ptList->Entries, unCount, sizeof(EntryList_ts), bUpDown ? List_SortCmpUp : List_SortCmpDown);
}

/*----------------------------------------------------------------------------*/
char* List_ToString(const List_ts *ptList, const char *c_pacDelimiter)
{
  uint16_t  unIdx;
  uint16_t  unItemCount = List_ItemCount(ptList);
  char *pacString = NULL;
/*
  for (unIdx = 0; unIdx < unItemCount; unIdx++)
  {
    uint16_t  unItemLen;
    uint8_t   *paucItem = List_GetItem(ptList, unIdx, &unItemLen);
    
    if (pacString == NULL)
      pacString = STR_cpy((char *)paucItem);
    else
      pacString = STR_cat(pacString, 2, c_pacDelimiter, (char *)paucItem);
  }
  */
  return pacString;
}

/*----------------------------------------------------------------------------*/
void List_Free(List_ts *ptList)
{
  uint16_t unI, unCount = List_ItemCount(ptList);

  for (unI = 0; unI < unCount; unI++)
  {
    uint8_t *Entry = List_GetItem(ptList, unI, NULL);
    free(Entry);
  }
  free(ptList->Entries);
  free(ptList);
}



