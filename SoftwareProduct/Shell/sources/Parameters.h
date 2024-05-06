#ifndef __LIST_H__
#define __LIST_H__

/** @defgroup   LIST  LIST-Group
                Implementation of a dynamic list.
    @{
********************************************************************************
*   @name       LIST                                                           
*   @file       LIST.h                                                         
* 
*   @brief      Implementation of a dynamic list.                                                             
*   
*   @authors    A.Bader                                                    
*   @date       21.05.2014                                                    
*                                                                             
*   @copyright (C) 2014 by: MICROTRONIC AG                             
*                           Duennernstrasse 32                                  
*                           CH-4702 Oensingen                                   
*******************************************************************************/

/*== Includes ================================================================*/
#include <stdint.h>
#include <stdbool.h>

/*== Defines =================================================================*/


/*== Structures ==============================================================*/

/// Every List Entry is managed with this struct
typedef struct EntryList_ts
{
  uint8_t*  Entry;                        ///< Pointer to List Entry
  uint16_t  Lenght;                           ///< Length of List Entry
} EntryList_ts;

/// The list is an instance of this struct. patEntries is separately allocated
/// to fetch unMaxNumber * sizof t_ListEntry.
typedef struct List_ts
{
  uint16_t        MaxNumber;                ///< Maximum Number of Entries in List
  uint16_t        Numbers;                  ///< Currently used Number of Entries
  EntryList_ts*   Entries;                 ///< Pointer to List Entries
} List_ts;


/*== Module Interfaces =======================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Create dynamic list. Call List_Free at the end to free up all
                created buffers!

    @param[in]  unMaxEntries   max number of entries
    @return     pointer to dynamic list object

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
List_ts *List_Create(uint16_t aMaxEntries);

/*----------------------------------------------------------------------------*/
/** @brief      Get number of entries added to a dynamic list.

    @param[in]  ptList   object of dynamic list
    @return     number of entries

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint16_t List_ItemCount(const List_ts* aList);

/*----------------------------------------------------------------------------*/
/** @brief      Insert new entry at first free place in the list.
                The number of items which can be added, is specified
                in List_Create.

    @param[in]  ptList    object of dynamic list
    @param[in]  c_paucBuf  pointer to data to add
    @param[in]  unLen     length of data to add
    @return     index of item

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint16_t List_AddItem(List_ts *ptList, const uint8_t *c_paucBuf, uint16_t unLen);

/*----------------------------------------------------------------------------*/
/** @brief      Delete Item with given index.

    @param[in]  ptList  object of dynamic list
    @param[in]  unIdx   item index to delete
    @return     0: Sucessfully deleted / 1: Error. Index not found

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint16_t List_DeleteItem(List_ts *ptList, uint16_t unIdx);

/*----------------------------------------------------------------------------*/
/** @brief      Get data of entry at given index.

    @param[in]  ptList  object of dynamic list
    @param[in]  unIdx   index of item to return
    @param[out] punLen  if not NULL, length of data of this entry
    @return     pointer to new buffer containing data of entry
                or NULL, if unIdx > ItemCount.

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint8_t* List_GetItem(const List_ts *ptList, uint16_t unIdx, uint16_t *punLen);

/*----------------------------------------------------------------------------*/
/** @brief      Get data of entry at given index. Allocate a new buffer and copy
                the content to this buffer. This buffer needs to be freed by user!

    @param[in]  ptList  object of dynamic list
    @param[in]  unIdx   index of item to return
    @param[out] punLen  if not NULL, length of data of this entry
    @return     pointer to new buffer containing data of entry
                or NULL, if unIdx > ItemCount.

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint8_t* List_CopyItem(const List_ts *ptList, uint16_t unIdx, uint16_t *punLen);

/*----------------------------------------------------------------------------*/
/** @brief      Search a string Item in the list. The search string may contain
                wildcards * or ?.

    @param[in]  ptList       object of dynamic list
    @param[in]  c_pacSearch  string to search
    @param[in]  unStartIdx   index of item, on which search starts
    @param[out] punFoundIdx  index of found item. May be NULL, if index
                             not needed. if no item found, punFoundIdx will
                             be unchanged.
    @return     Pointer to data of found entry. If no item found,
                NULL is returned.

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint8_t* List_SearchStrItem(const List_ts *ptList, const char *c_pacSearch, uint16_t unStartIdx, uint16_t *punFoundIdx);

/*----------------------------------------------------------------------------*/
/** @brief      Search a string Item in the list. The items in the list may
                contain wildcards * or ?. (in opposite to List_SearchStrItem!!)

    @param[in]  ptList       object of dynamic list
    @param[in]  c_pacSearch  string to search
    @param[in]  unStartIdx   index of item, on which search starts
    @param[out] punFoundIdx  index of found item. May be NULL, if index
                             not needed. if no item found, punFoundIdx will
                             be unchanged.
    @return     Pointer to data of found entry. If no item found,
                NULL is returned.

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint8_t* List_SearchMaskStr(const List_ts *ptList, const char *c_pacSearch, uint16_t unStartIdx, uint16_t *punFoundIdx);

/*----------------------------------------------------------------------------*/
/** @brief      Create dynamic list. Call List_Free at the end to free up all
                created buffers!

    @param[in]  ptList       object of dynamic list
    @param[in]  c_pacSearch  string to search
    @param[in]  unStartIdx   index of item, on which search starts
    @return     number of items deleted

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint16_t List_DeleteStrItems(List_ts *ptList, const char *c_pacSearch, uint16_t unStartIdx);

/*----------------------------------------------------------------------------*/
/** @brief      Create dynamic list. Call List_Free at the end to free up all
                created buffers!

    @param[in]  ptList    object of dynamic list
    @param[in]  bUpDown   0 = highest weighted entry at index 0; 1 = vice versa
    @return     None

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
void List_Sort(List_ts *ptList, bool bUpDown);

/*----------------------------------------------------------------------------*/
/** @brief      Generates a delimited string from the list items.
                This is useful to get a string again after using STR_split.
                The string is dynamically allocated on HEAP.

    @param[in]  ptList          object of dynamic list
    @param[in]  c_pacDelimiter  delimiting string between elements
    @return     pointer to allocated string

    @author     A.Bader                                     @date 15.07.2014  */
/*----------------------------------------------------------------------------*/
char *List_ToString(const List_ts *ptList, const char *c_pacDelimiter);

/*----------------------------------------------------------------------------*/
/** @brief      Free up all allocated buffers which belog to ptList

    @param[in]  ptList    object of dynamic list
    @return     None

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
void List_Free(List_ts *ptList);

/// @}   // End of Group

#endif   // __List_H__