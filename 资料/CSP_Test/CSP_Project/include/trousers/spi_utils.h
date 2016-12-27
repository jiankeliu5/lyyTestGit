
/*
 * Licensed Materials - Property of IBM
 *
 * trousers - An open source TCG Software Stack
 *
 * (C) Copyright International Business Machines Corp. 2004, 2007
 *
 */


#ifndef _SPI_UTILS_H_
#define _SPI_UTILS_H_

#include "threads.h"
#ifndef WIN32
#include <netinet/in.h> // for endian routines
#endif
#include "trousers_types.h"
#include "trousers/trousers.h"

struct key_mem_cache
{
    TCS_KEY_HANDLE tcs_handle;
    TSS_HKEY tsp_handle;
    UINT16 flags;
    UINT32 time_stamp;
    TSS_UUID uuid;
    TSS_UUID p_uuid;
    TSS_KEY *blob;
    struct key_mem_cache *parent;
    struct key_mem_cache *next;
};

extern struct key_mem_cache *key_mem_cache_head;
MUTEX_DECLARE_EXTERN(mem_cache_lock);

#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define BOOL(x)        ((x) == 0) ? FALSE : TRUE
#define INVBOOL(x)    ((x) == 0) ? TRUE : FALSE

#define TSS_PSFILE_INCREMENT_NUM_KEYS    1
#define TSS_PSFILE_DECREMENT_NUM_KEYS    0

void *calloc_tspi(TSS_HCONTEXT, UINT32);
TSS_RESULT free_tspi(TSS_HCONTEXT, void *);
TSS_RESULT add_mem_entry(TSS_HCONTEXT, void *);

/* secrets.c */

TSS_RESULT policy_UsesAuth(TSS_HPOLICY, TSS_BOOL *);

TSS_RESULT secret_PerformAuth_OIAP(TSS_HOBJECT, UINT32, TSS_HPOLICY, TSS_BOOL, TCPA_DIGEST *,
                   TPM_AUTH *);
TSS_RESULT secret_PerformXOR_OSAP(TSS_HPOLICY, TSS_HPOLICY, TSS_HPOLICY, TSS_HOBJECT,
                  UINT16, UINT32, TCPA_ENCAUTH *, TCPA_ENCAUTH *,
                  BYTE *, TPM_AUTH *, TCPA_NONCE *);
TSS_RESULT secret_PerformAuth_OSAP(TSS_HOBJECT, UINT32, TSS_HPOLICY,
                   TSS_HPOLICY, TSS_HPOLICY, BYTE *,
                   TPM_AUTH *, BYTE *, TCPA_NONCE *);

TSS_RESULT secret_ValidateAuth_OSAP(TSS_HOBJECT, UINT32, TSS_HPOLICY,
                    TSS_HPOLICY, TSS_HPOLICY, BYTE *,
                    TPM_AUTH *, BYTE *, TCPA_NONCE *);
TSS_RESULT secret_TakeOwnership(TSS_HKEY, TSS_HTPM, TSS_HKEY, TPM_AUTH *,
                UINT32 *, BYTE *, UINT32 *, BYTE *);

TSS_RESULT changeauth_owner(TSS_HCONTEXT, TSS_HOBJECT, TSS_HOBJECT, TSS_HPOLICY);
TSS_RESULT changeauth_srk(TSS_HCONTEXT, TSS_HOBJECT, TSS_HOBJECT, TSS_HPOLICY);
TSS_RESULT changeauth_key(TSS_HCONTEXT, TSS_HOBJECT, TSS_HOBJECT, TSS_HPOLICY);
TSS_RESULT changeauth_encdata(TSS_HCONTEXT, TSS_HOBJECT, TSS_HOBJECT, TSS_HPOLICY);

TSS_RESULT sealx_mask_cb(PVOID, TSS_HKEY, TSS_HENCDATA, TSS_ALGORITHM_ID, UINT32, BYTE *, BYTE *,
             BYTE *, BYTE *, UINT32, BYTE *, BYTE *);

TSS_RESULT free_resource(TSS_HCONTEXT, UINT32, UINT32);
TSS_RESULT owner_get_pubek(TSS_HCONTEXT, TSS_HTPM, TSS_HKEY *);

#define next( x )    x = x->next

/* spi_utils.c */

UINT16 get_num_pcrs(TSS_HCONTEXT);
void   free_key_refs(TSS_KEY *);

#define UI_MAX_SECRET_STRING_LENGTH    256
#define UI_MAX_POPUP_STRING_LENGTH    256

#ifdef TSS_NO_GUI
#define DisplayPINWindow(a,b,c)            \
    do {                    \
        *(b) = 0;            \
    } while (0)
#define DisplayNewPINWindow(a,b,c)            \
    do {                    \
        *(b) = 0;            \
    } while (0)
#else
TSS_RESULT DisplayPINWindow(BYTE *, UINT32 *, BYTE *);
TSS_RESULT DisplayNewPINWindow(BYTE *, UINT32 *, BYTE *);
#endif

TSS_RESULT merge_key_hierarchies(TSS_HCONTEXT, UINT32, TSS_KM_KEYINFO *, UINT32, TSS_KM_KEYINFO *,
                 UINT32 *, TSS_KM_KEYINFO **);
TSS_RESULT merge_key_hierarchies2(TSS_HCONTEXT, UINT32, TSS_KM_KEYINFO2 *, UINT32, TSS_KM_KEYINFO2 *,
                 UINT32 *, TSS_KM_KEYINFO2 **);


int pin_mem(void *, size_t);
int unpin_mem(void *, size_t);


#define TSS_MAX_SYM_BLOCK_SIZE    16

TSS_RESULT internal_GetCap(TSS_HCONTEXT, TSS_FLAG, UINT32, UINT32 *, BYTE **);

/* For an unconnected context that wants to do PCR operations, assume that
 * the TPM has TSS_DEFAULT_NUM_PCRS pcrs */
#define TSS_DEFAULT_NUM_PCRS        16
#define TSS_LOCAL_RANDOM_DEVICE        "/dev/urandom"
#define TSS_LOCALHOST_STRING        "localhost"
TSS_RESULT get_local_random(TSS_HCONTEXT, TSS_BOOL, UINT32, BYTE **);

#define AUTH_RETRY_NANOSECS    500000000
#define AUTH_RETRY_COUNT    5
#define TPM_AUTH_RQU_SIZE    (sizeof(TPM_AUTHHANDLE) + sizeof(TPM_NONCE) \
                 + sizeof(TPM_BOOL) + sizeof(TPM_AUTHDATA))
#define TPM_AUTH_RSP_SIZE    (sizeof(TPM_NONCE) + sizeof(TPM_BOOL) + sizeof(TPM_AUTHDATA))

#define endian32(x)    htonl(x)
#define endian16(x)    htons(x)

//#ifdef USES_TCG_STD
extern TSS_VERSION VERSION_1_1;
//#else
extern TSM_VERSION VERSION_1_0;
//#endif

TSS_RESULT rsa_encrypt(TSS_HKEY, UINT32, BYTE*, UINT32*, BYTE*);
TSS_RESULT rsa_verify(TSS_HKEY, UINT32, UINT32, BYTE*, UINT32, BYTE*);

TSS_RESULT Init_AuthNonce(TCS_CONTEXT_HANDLE, TSS_BOOL, TPM_AUTH *);
TSS_BOOL validateReturnAuth(BYTE *, BYTE *, TPM_AUTH *);
void HMAC_Auth(BYTE *, BYTE *, TPM_AUTH *);

void test(BYTE * secret, BYTE * Digest, TPM_AUTH * auth);
TSM_RESULT test2(BYTE * secret, BYTE * Digest, TPM_AUTH * auth);

TSS_RESULT OSAP_Calc(TCS_CONTEXT_HANDLE, UINT16, UINT32, BYTE *, BYTE *, BYTE *,
            TCPA_ENCAUTH *, TCPA_ENCAUTH *, BYTE *, TPM_AUTH *);

void UINT64ToArray(UINT64, BYTE *);
void UINT32ToArray(UINT32, BYTE *);
void UINT16ToArray(UINT16, BYTE *);
UINT16 Decode_UINT16(BYTE *);
UINT32 Decode_UINT32(BYTE *);
UINT64 Decode_UINT64(BYTE *);

TSS_RESULT popup_GetSecret(UINT32, UINT32, BYTE *, void *);

TSS_RESULT get_tpm_flags(TSS_HCONTEXT, TSS_HTPM, UINT32 *, UINT32 *);
TSS_RESULT pcrs_calc_composite(TCPA_PCR_SELECTION *, TCPA_PCRVALUE *, TCPA_DIGEST *);
struct tr_pcrs_obj;
TSS_RESULT pcrs_sanity_check_selection(TCS_CONTEXT_HANDLE, struct tr_pcrs_obj *, TPM_PCR_SELECTION *);

void LoadBlob_AUTH(UINT64 *, BYTE *, TPM_AUTH *);
void UnloadBlob_AUTH(UINT64 *, BYTE *, TPM_AUTH *);
void LoadBlob_LOADKEY_INFO(UINT64 *, BYTE *, TCS_LOADKEY_INFO *);
void UnloadBlob_LOADKEY_INFO(UINT64 *, BYTE *, TCS_LOADKEY_INFO *);
void LoadBlob_TSS_KEY(UINT64 *, BYTE *, TSS_KEY *);
TSS_RESULT UnloadBlob_TSS_KEY(UINT64 *, BYTE *, TSS_KEY *);
TSS_RESULT Hash_TSS_KEY(Trspi_HashCtx *, TSS_KEY *);
void LoadBlob_TSS_PRIVKEY_DIGEST(UINT64 *, BYTE *, TSS_KEY *);
TSS_RESULT Hash_TSS_PRIVKEY_DIGEST(Trspi_HashCtx *, TSS_KEY *);

TSS_RESULT TSP_SetCapability(TSS_HCONTEXT, TSS_HTPM, TSS_HPOLICY, TPM_CAPABILITY_AREA,
                 UINT32, TSS_BOOL);

TSS_RESULT RPC_OpenContext(TSS_HCONTEXT, BYTE *, int);
TSS_RESULT RPC_FreeMemory(TSS_HCONTEXT, BYTE *);
TSS_RESULT RPC_GetRegisteredKeyByPublicInfo(TSS_HCONTEXT, TCPA_ALGORITHM_ID, UINT32,
                                              BYTE *, UINT32 *, BYTE **);
TSS_RESULT RPC_CloseContext(TSS_HCONTEXT);
TSS_RESULT RPC_GetCapability(TSS_HCONTEXT, TCPA_CAPABILITY_AREA, UINT32, BYTE *, UINT32 *, BYTE **);
TSS_RESULT RPC_GetTPMCapability(TSS_HCONTEXT, TCPA_CAPABILITY_AREA, UINT32, BYTE *, UINT32 *, BYTE **);
TSS_RESULT Transport_GetTPMCapability(TSS_HCONTEXT, TCPA_CAPABILITY_AREA, UINT32, BYTE *, UINT32 *, BYTE **);
TSS_RESULT RPC_SetCapability(TSS_HCONTEXT, TCPA_CAPABILITY_AREA, UINT32, BYTE *, UINT32, BYTE *, TPM_AUTH *);
TSS_RESULT Transport_SetCapability(TSS_HCONTEXT, TCPA_CAPABILITY_AREA,    UINT32, BYTE *, UINT32,
                   BYTE *, TPM_AUTH *);
TSS_RESULT RPC_LoadKeyByBlob(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *,
                               TCS_KEY_HANDLE *, TCS_KEY_HANDLE *);
TSS_RESULT Transport_LoadKeyByBlob(TSS_HCONTEXT, TSS_HKEY, UINT32, BYTE *,
                   TPM_AUTH *, TCS_KEY_HANDLE *, TPM_KEY_HANDLE *);
TSS_RESULT RPC_LoadKeyByUUID(TSS_HCONTEXT, TSS_UUID, TCS_LOADKEY_INFO *, TCS_KEY_HANDLE *);
TSS_RESULT RPC_GetRegisteredKey(TSS_HCONTEXT, TSS_UUID, TSS_KM_KEYINFO **);
TSS_RESULT RPC_GetRegisteredKeyBlob(TSS_HCONTEXT, TSS_UUID, UINT32 *, BYTE **);
TSS_RESULT RPC_RegisterKey(TSS_HCONTEXT, TSS_UUID, TSS_UUID, UINT32, BYTE *, UINT32, BYTE *);
TSS_RESULT RPC_UnregisterKey(TSS_HCONTEXT, TSS_UUID);
TSS_RESULT RPC_EnumRegisteredKeys(TSS_HCONTEXT, TSS_UUID *, UINT32 *, TSS_KM_KEYINFO **);
TSS_RESULT RPC_EnumRegisteredKeys2(TSS_HCONTEXT, TSS_UUID *, UINT32 *, TSS_KM_KEYINFO2 **);
TSS_RESULT RPC_ChangeAuth(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_PROTOCOL_ID, TCPA_ENCAUTH *,
                TCPA_ENTITY_TYPE, UINT32, BYTE *, TPM_AUTH *, TPM_AUTH *,
                            UINT32 *, BYTE **);
TSS_RESULT Transport_ChangeAuth(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_PROTOCOL_ID, TCPA_ENCAUTH *,
                TCPA_ENTITY_TYPE, UINT32, BYTE *, TPM_AUTH *, TPM_AUTH *,
                            UINT32 *, BYTE **);
TSS_RESULT RPC_ChangeAuthOwner(TSS_HCONTEXT, TCPA_PROTOCOL_ID, TCPA_ENCAUTH *, TCPA_ENTITY_TYPE,
                                 TPM_AUTH *);
TSS_RESULT Transport_ChangeAuthOwner(TSS_HCONTEXT, TCPA_PROTOCOL_ID, TCPA_ENCAUTH *, TCPA_ENTITY_TYPE,
                                 TPM_AUTH *);
TSS_RESULT RPC_TerminateHandle(TSS_HCONTEXT, TCS_AUTHHANDLE);
TSS_RESULT Transport_TerminateHandle(TSS_HCONTEXT, TCS_AUTHHANDLE);
TSS_RESULT RPC_GetRandom(TSS_HCONTEXT, UINT32, BYTE **);
TSS_RESULT Transport_GetRandom(TSS_HCONTEXT, UINT32, BYTE **);
TSS_RESULT RPC_ChangeAuthAsymStart(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE, UINT32, BYTE *,
                                     TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **, UINT32 *,
                                     BYTE **, TCS_KEY_HANDLE *);
TSS_RESULT RPC_ChangeAuthAsymFinish(TSS_HCONTEXT, TCS_KEY_HANDLE, TCS_KEY_HANDLE,
                    TCPA_ENTITY_TYPE, TCPA_HMAC, UINT32, BYTE *, UINT32,
                    BYTE *, TPM_AUTH *, UINT32 *, BYTE **, TCPA_SALT_NONCE *,
                    TCPA_DIGEST *);
TSS_RESULT RPC_GetPubKey(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_GetPubKey(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_CreateWrapKey(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_ENCAUTH *, TCPA_ENCAUTH *,
                UINT32, BYTE *, UINT32 *, BYTE **, TPM_AUTH *);
TSS_RESULT Transport_CreateWrapKey(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_ENCAUTH *, TCPA_ENCAUTH *,
                   UINT32, BYTE *, UINT32 *, BYTE **, TPM_AUTH *);
TSS_RESULT RPC_CertifyKey(TSS_HCONTEXT, TCS_KEY_HANDLE, TCS_KEY_HANDLE, TPM_NONCE *, TPM_AUTH *,
                TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT Transport_CertifyKey(TSS_HCONTEXT, TCS_KEY_HANDLE, TCS_KEY_HANDLE, TPM_NONCE *,
                TPM_AUTH *, TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_CreateMigrationBlob(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_MIGRATE_SCHEME, UINT32,
                    BYTE *, UINT32, BYTE *, TPM_AUTH *, TPM_AUTH *, UINT32 *,
                    BYTE **, UINT32 *, BYTE **);
TSS_RESULT Transport_CreateMigrationBlob(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_MIGRATE_SCHEME, UINT32,
                    BYTE *, UINT32, BYTE *, TPM_AUTH *, TPM_AUTH *, UINT32 *,
                    BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_ConvertMigrationBlob(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, UINT32,
                     BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_ConvertMigrationBlob(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, UINT32,
                     BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_PcrRead(TSS_HCONTEXT, TCPA_PCRINDEX, TCPA_PCRVALUE *);
TSS_RESULT Transport_PcrRead(TSS_HCONTEXT, TCPA_PCRINDEX, TCPA_PCRVALUE *);
TSS_RESULT RPC_PcrReset(TSS_HCONTEXT, UINT32, BYTE *);
TSS_RESULT Transport_PcrReset(TSS_HCONTEXT, UINT32, BYTE *);
TSS_RESULT RPC_OSAP(TSS_HCONTEXT, TCPA_ENTITY_TYPE, UINT32, TPM_NONCE *, TCS_AUTHHANDLE *,
            TCPA_NONCE *, TCPA_NONCE *);
TSS_RESULT Transport_OSAP(TSS_HCONTEXT, TCPA_ENTITY_TYPE, UINT32, TPM_NONCE *, TCS_AUTHHANDLE *,
              TCPA_NONCE *, TCPA_NONCE *);
TSS_RESULT RPC_GetCapabilityOwner(TSS_HCONTEXT, TPM_AUTH *, TCPA_VERSION *, UINT32 *, UINT32 *);
TSS_RESULT Transport_GetCapabilityOwner(TSS_HCONTEXT, TPM_AUTH *, TCPA_VERSION *, UINT32 *, UINT32 *);
TSS_RESULT RPC_OIAP(TSS_HCONTEXT, TCS_AUTHHANDLE *, TCPA_NONCE *);
TSS_RESULT Transport_OIAP(TSS_HCONTEXT, TCS_AUTHHANDLE *, TCPA_NONCE *);
TSS_RESULT RPC_Seal(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_ENCAUTH *, UINT32, BYTE *, UINT32, BYTE *,
            TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_Seal(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_ENCAUTH *, UINT32, BYTE *, UINT32,
              BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_Sealx(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_ENCAUTH *, UINT32, BYTE *, UINT32, BYTE *,
                                       TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_Sealx(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_ENCAUTH *, UINT32, BYTE *, UINT32,
               BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_Unseal(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *, TPM_AUTH *,
                                         UINT32 *, BYTE **);
TSS_RESULT Transport_Unseal(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *, TPM_AUTH *,
                UINT32 *, BYTE **);
TSS_RESULT RPC_UnBind(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *, UINT32 *,
                                         BYTE **);
TSS_RESULT Transport_UnBind(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *, UINT32 *,
                                         BYTE **);
TSS_RESULT RPC_Sign(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_Sign(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_CreateEndorsementKeyPair(TSS_HCONTEXT, TCPA_NONCE, UINT32, BYTE *, UINT32 *,
                        BYTE **, TCPA_DIGEST *);
TSS_RESULT RPC_ReadPubek(TSS_HCONTEXT, TCPA_NONCE, UINT32 *, BYTE **, TCPA_DIGEST *);
TSS_RESULT RPC_OwnerReadPubek(TSS_HCONTEXT, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_TakeOwnership(TSS_HCONTEXT, UINT16, UINT32, BYTE *, UINT32, BYTE *, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_CreateRevocableEndorsementKeyPair(TSS_HCONTEXT, TPM_NONCE, UINT32, BYTE *, TSS_BOOL, TPM_DIGEST *, UINT32 *, BYTE **, TPM_DIGEST *);
TSS_RESULT RPC_RevokeEndorsementKeyPair(TSS_HCONTEXT, TPM_DIGEST *);
TSS_RESULT RPC_MakeIdentity(TSS_HCONTEXT, TCPA_ENCAUTH, TCPA_CHOSENID_HASH, UINT32, BYTE *,
                TPM_AUTH *, TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **, UINT32 *,
                BYTE **, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT Transport_MakeIdentity2(TSS_HCONTEXT, TCPA_ENCAUTH, TCPA_CHOSENID_HASH, UINT32, BYTE *, TPM_AUTH *, TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_ActivateTPMIdentity(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *,
                    TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_ActivateTPMIdentity(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *,
                     TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_OwnerClear(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT Transport_OwnerClear(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT RPC_ForceClear(TSS_HCONTEXT);
TSS_RESULT Transport_ForceClear(TSS_HCONTEXT);
TSS_RESULT RPC_DisableOwnerClear(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT Transport_DisableOwnerClear(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT RPC_DisableForceClear(TSS_HCONTEXT);
TSS_RESULT Transport_DisableForceClear(TSS_HCONTEXT);
TSS_RESULT RPC_PhysicalDisable(TSS_HCONTEXT);
TSS_RESULT Transport_PhysicalDisable(TSS_HCONTEXT);
TSS_RESULT RPC_PhysicalEnable(TSS_HCONTEXT);
TSS_RESULT Transport_PhysicalEnable(TSS_HCONTEXT);
TSS_RESULT RPC_PhysicalSetDeactivated(TSS_HCONTEXT, TSS_BOOL);
TSS_RESULT Transport_PhysicalSetDeactivated(TSS_HCONTEXT, TSS_BOOL);
TSS_RESULT RPC_PhysicalPresence(TSS_HCONTEXT, TCPA_PHYSICAL_PRESENCE);
TSS_RESULT Transport_PhysicalPresence(TSS_HCONTEXT, TCPA_PHYSICAL_PRESENCE);
TSS_RESULT RPC_SetTempDeactivated(TSS_HCONTEXT);
TSS_RESULT Transport_SetTempDeactivated(TSS_HCONTEXT);
TSS_RESULT RPC_SetTempDeactivated2(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT Transport_SetTempDeactivated2(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT RPC_OwnerSetDisable(TSS_HCONTEXT, TSS_BOOL, TPM_AUTH *);
TSS_RESULT Transport_OwnerSetDisable(TSS_HCONTEXT, TSS_BOOL, TPM_AUTH *);
TSS_RESULT RPC_ResetLockValue(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT Transport_ResetLockValue(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT RPC_SetOwnerInstall(TSS_HCONTEXT, TSS_BOOL);
TSS_RESULT Transport_SetOwnerInstall(TSS_HCONTEXT, TSS_BOOL);
TSS_RESULT RPC_DisablePubekRead(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT Transport_DisablePubekRead(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT RPC_SelfTestFull(TSS_HCONTEXT);
TSS_RESULT Transport_SelfTestFull(TSS_HCONTEXT);
TSS_RESULT RPC_CertifySelfTest(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE, TPM_AUTH *, UINT32 *,
                BYTE **);
TSS_RESULT Transport_CertifySelfTest(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE, TPM_AUTH *, UINT32 *,
                BYTE **);
TSS_RESULT RPC_GetTestResult(TSS_HCONTEXT, UINT32 *, BYTE **);
TSS_RESULT Transport_GetTestResult(TSS_HCONTEXT, UINT32 *, BYTE **);
TSS_RESULT RPC_StirRandom(TSS_HCONTEXT, UINT32, BYTE *);
TSS_RESULT Transport_StirRandom(TSS_HCONTEXT, UINT32, BYTE *);
TSS_RESULT RPC_AuthorizeMigrationKey(TSS_HCONTEXT, TCPA_MIGRATE_SCHEME, UINT32, BYTE *,
                    TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_AuthorizeMigrationKey(TSS_HCONTEXT, TCPA_MIGRATE_SCHEME, UINT32, BYTE *,
                    TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_GetPcrEvent(TSS_HCONTEXT, UINT32, UINT32 *, TSS_PCR_EVENT **);
TSS_RESULT RPC_GetPcrEventsByPcr(TSS_HCONTEXT, UINT32, UINT32, UINT32 *, TSS_PCR_EVENT **);
TSS_RESULT RPC_GetPcrEventLog(TSS_HCONTEXT, UINT32 *, TSS_PCR_EVENT **);
TSS_RESULT RPC_Quote(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE *, UINT32, BYTE *, TPM_AUTH *,
            UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT Transport_Quote(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE *, UINT32, BYTE *, TPM_AUTH *,
            UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_Quote2(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE *, UINT32, BYTE *, TSS_BOOL, TPM_AUTH *,
            UINT32 *, BYTE **, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT Transport_Quote2(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE *, UINT32, BYTE *, TSS_BOOL,
                 TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_Extend(TSS_HCONTEXT, TCPA_PCRINDEX, TCPA_DIGEST, TCPA_PCRVALUE *);
TSS_RESULT Transport_Extend(TSS_HCONTEXT, TCPA_PCRINDEX, TCPA_DIGEST, TCPA_PCRVALUE *);
TSS_RESULT RPC_DirWriteAuth(TSS_HCONTEXT, TCPA_DIRINDEX, TCPA_DIRVALUE *, TPM_AUTH *);
TSS_RESULT Transport_DirWriteAuth(TSS_HCONTEXT, TCPA_DIRINDEX, TCPA_DIRVALUE *, TPM_AUTH *);
TSS_RESULT RPC_DirRead(TSS_HCONTEXT, TCPA_DIRINDEX, TCPA_DIRVALUE *);
TSS_RESULT Transport_DirRead(TSS_HCONTEXT, TCPA_DIRINDEX, TCPA_DIRVALUE *);
TSS_RESULT RPC_LogPcrEvent(TSS_HCONTEXT, TSS_PCR_EVENT, UINT32 *);
TSS_RESULT RPC_EvictKey(TSS_HCONTEXT, TCS_KEY_HANDLE);
TSS_RESULT Transport_EvictKey(TSS_HCONTEXT, TCS_KEY_HANDLE);
TSS_RESULT RPC_CreateMaintenanceArchive(TSS_HCONTEXT, TSS_BOOL, TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT Transport_CreateMaintenanceArchive(TSS_HCONTEXT, TSS_BOOL, TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_KillMaintenanceFeature(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT Transport_KillMaintenanceFeature(TSS_HCONTEXT, TPM_AUTH *);
TSS_RESULT RPC_LoadMaintenanceArchive(TSS_HCONTEXT, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_LoadMaintenanceArchive(TSS_HCONTEXT, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_LoadManuMaintPub(TSS_HCONTEXT, TCPA_NONCE, UINT32, BYTE *, TCPA_DIGEST *);
TSS_RESULT Transport_LoadManuMaintPub(TSS_HCONTEXT, TCPA_NONCE, UINT32, BYTE *, TCPA_DIGEST *);
TSS_RESULT RPC_ReadManuMaintPub(TSS_HCONTEXT, TCPA_NONCE, TCPA_DIGEST *);
TSS_RESULT Transport_ReadManuMaintPub(TSS_HCONTEXT, TCPA_NONCE, TCPA_DIGEST *);
TSS_RESULT RPC_DaaJoin(TSS_HCONTEXT,  TPM_HANDLE, BYTE, UINT32, BYTE *, UINT32, BYTE *,
            TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_DaaSign(TSS_HCONTEXT,  TPM_HANDLE, BYTE, UINT32, BYTE *, UINT32, BYTE *,
            TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_ReadCounter(TSS_HCONTEXT, TSS_COUNTER_ID, TPM_COUNTER_VALUE *);
TSS_RESULT Transport_ReadCounter(TSS_HCONTEXT, TSS_COUNTER_ID, TPM_COUNTER_VALUE *);
TSS_RESULT RPC_CreateCounter(TSS_HCONTEXT, UINT32, BYTE *, TPM_ENCAUTH, TPM_AUTH *,
                  TSS_COUNTER_ID *, TPM_COUNTER_VALUE *);
TSS_RESULT RPC_IncrementCounter(TSS_HCONTEXT, TSS_COUNTER_ID, TPM_AUTH *, TPM_COUNTER_VALUE *);
TSS_RESULT RPC_ReleaseCounter(TSS_HCONTEXT, TSS_COUNTER_ID, TPM_AUTH *);
TSS_RESULT RPC_ReleaseCounterOwner(TSS_HCONTEXT, TSS_COUNTER_ID, TPM_AUTH *);
TSS_RESULT RPC_ReadCurrentTicks(TSS_HCONTEXT, UINT32 *, BYTE **);
TSS_RESULT Transport_ReadCurrentTicks(TSS_HCONTEXT, UINT32 *, BYTE **);
TSS_RESULT RPC_TickStampBlob(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_NONCE *, TPM_DIGEST *, TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT Transport_TickStampBlob(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_NONCE *, TPM_DIGEST *, TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_EstablishTransport(TSS_HCONTEXT, UINT32, TCS_KEY_HANDLE, UINT32, BYTE *, UINT32, BYTE *, TPM_AUTH *, TPM_MODIFIER_INDICATOR *, TCS_HANDLE *, UINT32 *, BYTE **, TPM_NONCE *);
TSS_RESULT RPC_ExecuteTransport(TSS_HCONTEXT, TPM_COMMAND_CODE, UINT32, BYTE *, UINT32 *, TCS_HANDLE **, TPM_AUTH *, TPM_AUTH *, TPM_AUTH *, UINT64 *, TPM_MODIFIER_INDICATOR *, TPM_RESULT *, UINT32 *, BYTE **);
TSS_RESULT RPC_ReleaseTransportSigned(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_NONCE *, TPM_AUTH *, TPM_AUTH *, TPM_MODIFIER_INDICATOR *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_NV_DefineOrReleaseSpace(TSS_HCONTEXT, UINT32, BYTE *, TCPA_ENCAUTH, TPM_AUTH *);
TSS_RESULT Transport_NV_DefineOrReleaseSpace(TSS_HCONTEXT, UINT32, BYTE *, TCPA_ENCAUTH, TPM_AUTH *);
TSS_RESULT RPC_NV_WriteValue(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32, BYTE *, TPM_AUTH *);
TSS_RESULT Transport_NV_WriteValue(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32, BYTE *, TPM_AUTH *);
TSS_RESULT RPC_NV_WriteValueAuth(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32, BYTE *, TPM_AUTH *);
TSS_RESULT Transport_NV_WriteValueAuth(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32, BYTE *, TPM_AUTH *);
TSS_RESULT RPC_NV_ReadValue(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32 *, TPM_AUTH *, BYTE **);
TSS_RESULT Transport_NV_ReadValue(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32 *, TPM_AUTH *, BYTE **);
TSS_RESULT RPC_NV_ReadValueAuth(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32 *, TPM_AUTH *, BYTE **);
TSS_RESULT Transport_NV_ReadValueAuth(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32 *, TPM_AUTH *, BYTE **);
TSS_RESULT RPC_SetOrdinalAuditStatus(TSS_HCONTEXT, TPM_AUTH *, UINT32, TSS_BOOL);
TSS_RESULT Transport_SetOrdinalAuditStatus(TSS_HCONTEXT, TPM_AUTH *, UINT32, TSS_BOOL);
TSS_RESULT RPC_GetAuditDigest(TSS_HCONTEXT, UINT32, TPM_DIGEST *, UINT32 *, BYTE **, TSS_BOOL *, UINT32 *, UINT32 **);
TSS_RESULT Transport_GetAuditDigest(TSS_HCONTEXT, UINT32, TPM_DIGEST *, UINT32 *, BYTE **, TSS_BOOL *, UINT32 *, UINT32 **);
TSS_RESULT RPC_GetAuditDigestSigned(TSS_HCONTEXT, TCS_KEY_HANDLE, TSS_BOOL, TPM_NONCE *,
                    TPM_AUTH *, UINT32 *, BYTE **, TPM_DIGEST *, TPM_DIGEST *,
                    UINT32 *, BYTE **);
TSS_RESULT Transport_GetAuditDigestSigned(TSS_HCONTEXT, TCS_KEY_HANDLE, TSS_BOOL, TPM_NONCE *,
                      TPM_AUTH *, UINT32 *, BYTE **, TPM_DIGEST *, TPM_DIGEST *,
                      UINT32 *, BYTE **);
TSS_RESULT RPC_SetOperatorAuth(TSS_HCONTEXT, TCPA_SECRET *);
TSS_RESULT Transport_SetOperatorAuth(TSS_HCONTEXT, TCPA_SECRET *);
TSS_RESULT RPC_OwnerReadInternalPub(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_OwnerReadInternalPub(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_KeyControlOwner(TCS_CONTEXT_HANDLE, TCS_KEY_HANDLE, UINT32, BYTE *, UINT32, TSS_BOOL, TPM_AUTH *, TSS_UUID *);
TSS_RESULT RPC_GetCredential(TSS_HCONTEXT, UINT32, UINT32, UINT32 *, BYTE **);
TSS_RESULT RPC_GetCapabilitySigned(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE, TCPA_CAPABILITY_AREA, UINT32, BYTE *, TPM_AUTH *, TCPA_VERSION *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_FieldUpgrade(TSS_HCONTEXT, UINT32, BYTE *, UINT32 *, BYTE **, TPM_AUTH *);
TSS_RESULT RPC_SetRedirection(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, UINT32, TPM_AUTH *);
TSS_RESULT RPC_Delegate_Manage(TSS_HCONTEXT, TPM_FAMILY_ID, TPM_FAMILY_OPERATION, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_Delegate_Manage(TSS_HCONTEXT, TPM_FAMILY_ID, TPM_FAMILY_OPERATION, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_Delegate_CreateKeyDelegation(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_ENCAUTH *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_Delegate_CreateKeyDelegation(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_ENCAUTH *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_Delegate_CreateOwnerDelegation(TSS_HCONTEXT, TSS_BOOL, UINT32, BYTE *, TPM_ENCAUTH *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_Delegate_CreateOwnerDelegation(TSS_HCONTEXT, TSS_BOOL, UINT32, BYTE *, TPM_ENCAUTH *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_Delegate_LoadOwnerDelegation(TSS_HCONTEXT, TPM_DELEGATE_INDEX, UINT32, BYTE *, TPM_AUTH *);
TSS_RESULT Transport_Delegate_LoadOwnerDelegation(TSS_HCONTEXT, TPM_DELEGATE_INDEX, UINT32, BYTE *, TPM_AUTH *);
TSS_RESULT RPC_Delegate_ReadTable(TSS_HCONTEXT, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT Transport_Delegate_ReadTable(TSS_HCONTEXT, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_Delegate_UpdateVerificationCount(TSS_HCONTEXT, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_Delegate_UpdateVerificationCount(TSS_HCONTEXT, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_Delegate_VerifyDelegation(TSS_HCONTEXT, UINT32, BYTE *);
TSS_RESULT Transport_Delegate_VerifyDelegation(TSS_HCONTEXT, UINT32, BYTE *);
TSS_RESULT RPC_DSAP(TSS_HCONTEXT, TPM_ENTITY_TYPE, TCS_KEY_HANDLE, TPM_NONCE *, UINT32, BYTE *, TCS_AUTHHANDLE *, TPM_NONCE *, TPM_NONCE *);
TSS_RESULT Transport_DSAP(TSS_HCONTEXT, TPM_ENTITY_TYPE, TCS_KEY_HANDLE, TPM_NONCE *, UINT32, BYTE *, TCS_AUTHHANDLE *, TPM_NONCE *, TPM_NONCE *);
TSS_RESULT RPC_CMK_SetRestrictions(TSS_HCONTEXT, TSS_CMK_DELEGATE, TPM_AUTH *);
TSS_RESULT Transport_CMK_SetRestrictions(TSS_HCONTEXT, TSS_CMK_DELEGATE, TPM_AUTH *);
TSS_RESULT RPC_CMK_ApproveMA(TSS_HCONTEXT, TPM_DIGEST, TPM_AUTH *, TPM_HMAC *);
TSS_RESULT Transport_CMK_ApproveMA(TSS_HCONTEXT, TPM_DIGEST, TPM_AUTH *, TPM_HMAC *);
TSS_RESULT RPC_CMK_CreateKey(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_ENCAUTH *, TPM_HMAC *, TPM_DIGEST *, UINT32 *, BYTE **, TPM_AUTH *);
TSS_RESULT Transport_CMK_CreateKey(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_ENCAUTH, TPM_HMAC, TPM_DIGEST, UINT32 *, BYTE **, TPM_AUTH *);
TSS_RESULT RPC_CMK_CreateTicket(TSS_HCONTEXT, UINT32, BYTE *, TPM_DIGEST, UINT32, BYTE *, TPM_AUTH *, TPM_HMAC *);
TSS_RESULT Transport_CMK_CreateTicket(TSS_HCONTEXT, UINT32, BYTE *, TPM_DIGEST, UINT32, BYTE *, TPM_AUTH *, TPM_HMAC *);
TSS_RESULT RPC_CMK_CreateBlob(TSS_HCONTEXT, TCS_KEY_HANDLE, TSS_MIGRATE_SCHEME, UINT32, BYTE *, TPM_DIGEST, UINT32, BYTE *, UINT32, BYTE *, UINT32, BYTE *, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT Transport_CMK_CreateBlob(TSS_HCONTEXT, TCS_KEY_HANDLE, TSS_MIGRATE_SCHEME, UINT32, BYTE *, TPM_DIGEST, UINT32, BYTE *, UINT32, BYTE *, UINT32, BYTE *, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **);
TSS_RESULT RPC_CMK_ConvertMigration(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_CMK_AUTH, TPM_HMAC, UINT32, BYTE *, UINT32, BYTE *, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT Transport_CMK_ConvertMigration(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_CMK_AUTH, TPM_HMAC, UINT32, BYTE *, UINT32, BYTE *, UINT32, BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
TSS_RESULT RPC_FlushSpecific(TSS_HCONTEXT, TCS_HANDLE, TPM_RESOURCE_TYPE);
TSS_RESULT Transport_FlushSpecific(TSS_HCONTEXT, TCS_HANDLE, TPM_RESOURCE_TYPE);

#ifndef USES_TCG_STD

TSM_RESULT RPC_APCreate(
    TCS_CONTEXT_HANDLE      hContext,           // in
    TCM_ENTITY_TYPE         entityType,         // in
    UINT32                  entityValue,        // in
    TCM_NONCE               callerNonce,        // in
    TCM_AUTHDATA*           pAuth,              // in, out
    TCS_AUTHHANDLE*         authHandle,         // out
    TCM_NONCE*              TcmNonce,           // out
    UINT32*                 AntiReplaySeq);

TSM_RESULT RPC_APTerminate(TCS_CONTEXT_HANDLE hContext, TCS_AUTHHANDLE authHandle, TPM_AUTH* pAuth);

TSM_RESULT RPC_SMS4Encrypt(
    TCS_CONTEXT_HANDLE      hContext,           // in
    TCS_KEY_HANDLE          enckeyHandle,       // in
    BYTE*                   IV,                 // in
    UINT32                  inDataSize,         // in
    BYTE*                   inData,             // in
    TCM_AUTH*               pEncAuth,           // in, out
    UINT32*                 outDataSize,        // out
    BYTE**                  outData             // out
    );

TSM_RESULT RPC_SMS4Decrypt(
    TCS_CONTEXT_HANDLE      hContext,           // in
    TCS_KEY_HANDLE          DnckeyHandle,       // in
    BYTE*                   IV,                 // in
    UINT32                  inDataSize,         // in
    BYTE*                   inData,             // in
    TCM_AUTH*               pEncAuth,           // in, out
    UINT32*                 outDataSize,        // out
    BYTE**                  outData             // out
    );

TSM_RESULT RPC_CollatePekRequest(
    TCS_CONTEXT_HANDLE      hContext,                       // in
    TCM_CHOSENID_HASH       IDLabel_PrivCAHash,             // in
    UINT32*                 pcEndorsementCredentialSize,    // out
    BYTE**                  prgbEndorsementCredential       // out
    );

TSM_RESULT RPC_ActivatePEK(
    TCS_CONTEXT_HANDLE      hContext,           // in
    TCM_ENCAUTH             KeyUsageAuth,       // in
    TCM_ENCAUTH             KeyMigrationAuth,   // in
    UINT32                  PEKKeyInfoSize,     // in
    BYTE*                   PEKKeyInfo,         // in
    UINT32                  PEKDataSize,        // in
    BYTE*                   PEKData,            // in
    UINT32                  EncSymKeySize,      // in
    BYTE*                   EncSymKey,          // in
    TCM_AUTH*               pSmkAuth,           // in, out
    TCM_AUTH*               pOwnerAuth,         // in, out
    UINT32*                 PekKeySize,         // out
    BYTE**                  PekKey              // out
    );

TSM_RESULT RPC_ActivatePEKCert(
    TCS_CONTEXT_HANDLE      hContext,           // in
    UINT32                  blobSize,           // in
    BYTE*                   blob,               // in
    TCM_AUTH*               ownerAuth,          // in, out
    UINT32*                 SymmetricKeySize,   // out
    BYTE**                  SymmetricKey        // out
    );

#endif

TSS_RESULT RPC_Error(TSS_HCONTEXT, ...);

struct tcs_api_table {
#ifdef TSS_BUILD_KEY
    TSS_RESULT (*LoadKeyByBlob)(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *,
                    TCS_KEY_HANDLE *, TCS_KEY_HANDLE *);
    TSS_RESULT (*EvictKey)(TSS_HCONTEXT, TCS_KEY_HANDLE);
    TSS_RESULT (*CreateWrapKey)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_ENCAUTH *, TCPA_ENCAUTH *,
                    UINT32, BYTE *, UINT32 *, BYTE **, TPM_AUTH *);
    TSS_RESULT (*GetPubKey)(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_AUTH *, UINT32 *, BYTE **);
    TSS_RESULT (*OwnerReadInternalPub)(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_AUTH*, UINT32*,
                       BYTE**);
#ifdef TSS_BUILD_CERTIFY
    TSS_RESULT (*CertifyKey)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCS_KEY_HANDLE, TPM_NONCE *,
                 TPM_AUTH *, TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **);
#endif
#endif
#ifdef TSS_BUILD_OWN
    TSS_RESULT (*OwnerClear)(TSS_HCONTEXT, TPM_AUTH *);
    TSS_RESULT (*ForceClear)(TSS_HCONTEXT);
#endif
#ifdef TSS_BUILD_AUTH
    TSS_RESULT (*OIAP)(TSS_HCONTEXT, TCS_AUTHHANDLE *, TCPA_NONCE *);
    TSS_RESULT (*OSAP)(TSS_HCONTEXT, TCPA_ENTITY_TYPE, UINT32, TPM_NONCE *, TCS_AUTHHANDLE *,
               TCPA_NONCE *, TCPA_NONCE *);
    TSS_RESULT (*TerminateHandle)(TSS_HCONTEXT, TCS_AUTHHANDLE);
#endif
#ifdef TSS_BUILD_CHANGEAUTH
    TSS_RESULT (*ChangeAuth)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_PROTOCOL_ID, TCPA_ENCAUTH *,
                 TCPA_ENTITY_TYPE, UINT32, BYTE *, TPM_AUTH *, TPM_AUTH *, UINT32 *,
                 BYTE **);
    TSS_RESULT (*ChangeAuthOwner)(TSS_HCONTEXT, TCPA_PROTOCOL_ID, TCPA_ENCAUTH *,
                      TCPA_ENTITY_TYPE, TPM_AUTH *);
    TSS_RESULT (*ChangeAuthAsymStart)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE, UINT32, BYTE *,
                      TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **,
                      UINT32 *, BYTE **, TCS_KEY_HANDLE *);
    TSS_RESULT (*ChangeAuthAsymFinish)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCS_KEY_HANDLE,
                       TCPA_ENTITY_TYPE, TCPA_HMAC, UINT32, BYTE *, UINT32,
                       BYTE *, TPM_AUTH *, UINT32 *, BYTE **, TPM_NONCE *,
                       TCPA_DIGEST *);
#endif
#ifdef TSS_BUILD_AIK
    TSS_RESULT (*ActivateTPMIdentity)(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *,
                      TPM_AUTH *, UINT32 *, BYTE **);
#endif
#ifdef TSS_BUILD_PCR_EXTEND
    TSS_RESULT (*Extend)(TSS_HCONTEXT, TCPA_PCRINDEX, TCPA_DIGEST, TCPA_PCRVALUE *);
    TSS_RESULT (*PcrRead)(TSS_HCONTEXT, TCPA_PCRINDEX, TCPA_PCRVALUE *);
    TSS_RESULT (*PcrReset)(TSS_HCONTEXT, UINT32, BYTE *);
#endif
#ifdef TSS_BUILD_QUOTE
    TSS_RESULT (*Quote)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE *, UINT32, BYTE *, TPM_AUTH *,
                UINT32 *, BYTE **, UINT32 *, BYTE **);
#endif
#ifdef TSS_BUILD_QUOTE2
    TSS_RESULT (*Quote2)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE *, UINT32, BYTE *, TSS_BOOL,
                 TPM_AUTH *, UINT32 *, BYTE **, UINT32 *, BYTE **, UINT32 *, BYTE **);
#endif
#ifdef TSS_BUILD_DIR
    TSS_RESULT (*DirWriteAuth)(TSS_HCONTEXT, TCPA_DIRINDEX, TCPA_DIRVALUE *, TPM_AUTH *);
    TSS_RESULT (*DirRead)(TSS_HCONTEXT, TCPA_DIRINDEX, TCPA_DIRVALUE *);
#endif
#ifdef TSS_BUILD_SEAL
    TSS_RESULT (*Seal)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_ENCAUTH *, UINT32, BYTE *, UINT32,
               BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
    TSS_RESULT (*Sealx)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_ENCAUTH *, UINT32, BYTE *, UINT32,
                BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
    TSS_RESULT (*Unseal)(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *, TPM_AUTH *,
                 UINT32 *, BYTE **);
#endif
#ifdef TSS_BUILD_BIND
    TSS_RESULT (*UnBind)(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *, UINT32 *,
                 BYTE **);
#endif
#ifdef TSS_BUILD_MIGRATION
    TSS_RESULT (*CreateMigrationBlob)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_MIGRATE_SCHEME,
                      UINT32, BYTE *, UINT32, BYTE *, TPM_AUTH *, TPM_AUTH *,
                      UINT32 *, BYTE **, UINT32 *, BYTE **);
    TSS_RESULT (*ConvertMigrationBlob)(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, UINT32,
                       BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
    TSS_RESULT (*AuthorizeMigrationKey)(TSS_HCONTEXT, TCPA_MIGRATE_SCHEME, UINT32, BYTE *,
                        TPM_AUTH *, UINT32 *, BYTE **);
#endif
#ifdef TSS_BUILD_SIGN
    TSS_RESULT (*Sign)(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *, TPM_AUTH *, UINT32 *,
               BYTE **);
#endif
#ifdef TSS_BUILD_RANDOM
    TSS_RESULT (*GetRandom)(TSS_HCONTEXT, UINT32*, BYTE **);
    TSS_RESULT (*StirRandom)(TSS_HCONTEXT, UINT32, BYTE *);
#endif
#ifdef TSS_BUILD_CAPS_TPM
    TSS_RESULT (*GetTPMCapability)(TSS_HCONTEXT, TCPA_CAPABILITY_AREA, UINT32, BYTE *, UINT32 *,
                       BYTE **);
    TSS_RESULT (*SetCapability)(TSS_HCONTEXT, TCPA_CAPABILITY_AREA, UINT32, BYTE *, UINT32,
                    BYTE *, TPM_AUTH *);
    TSS_RESULT (*GetCapabilityOwner)(TSS_HCONTEXT, TPM_AUTH *, TCPA_VERSION *, UINT32 *,
                     UINT32 *);
#endif
#ifdef TSS_BUILD_EK
    TSS_RESULT (*CreateEndorsementKeyPair)(TSS_HCONTEXT, TCPA_NONCE, UINT32, BYTE *, UINT32 *,
                           BYTE **, TCPA_DIGEST *);
    TSS_RESULT (*ReadPubek)(TSS_HCONTEXT, TCPA_NONCE, UINT32 *, BYTE **, TCPA_DIGEST *);
    TSS_RESULT (*OwnerReadPubek)(TSS_HCONTEXT, TPM_AUTH *, UINT32 *, BYTE **);
#endif
#ifdef TSS_BUILD_SELFTEST
    TSS_RESULT (*SelfTestFull)(TSS_HCONTEXT);
    TSS_RESULT (*CertifySelfTest)(TSS_HCONTEXT, TCS_KEY_HANDLE, TCPA_NONCE, TPM_AUTH *,
                      UINT32 *, BYTE **);
    TSS_RESULT (*GetTestResult)(TSS_HCONTEXT, UINT32 *, BYTE **);
#endif
#ifdef TSS_BUILD_ADMIN
    TSS_RESULT (*DisablePubekRead)(TSS_HCONTEXT, TPM_AUTH *);
    TSS_RESULT (*SetOwnerInstall)(TSS_HCONTEXT, TSS_BOOL);
    TSS_RESULT (*OwnerSetDisable)(TSS_HCONTEXT, TSS_BOOL, TPM_AUTH *);
    TSS_RESULT (*ResetLockValue)(TSS_HCONTEXT, TPM_AUTH *);
    TSS_RESULT (*DisableOwnerClear)(TSS_HCONTEXT, TPM_AUTH *);
    TSS_RESULT (*DisableForceClear)(TSS_HCONTEXT);
    TSS_RESULT (*PhysicalDisable)(TSS_HCONTEXT);
    TSS_RESULT (*PhysicalEnable)(TSS_HCONTEXT);
    TSS_RESULT (*PhysicalSetDeactivated)(TSS_HCONTEXT, TSS_BOOL);
    TSS_RESULT (*PhysicalPresence)(TSS_HCONTEXT, TCPA_PHYSICAL_PRESENCE);
    TSS_RESULT (*SetTempDeactivated)(TSS_HCONTEXT);
    TSS_RESULT (*SetTempDeactivated2)(TSS_HCONTEXT, TPM_AUTH *);
#endif
#ifdef TSS_BUILD_MAINT
    TSS_RESULT (*CreateMaintenanceArchive)(TSS_HCONTEXT, TSS_BOOL, TPM_AUTH *, UINT32 *,
                           BYTE **, UINT32 *, BYTE **);
    TSS_RESULT (*LoadMaintenanceArchive)(TSS_HCONTEXT, UINT32, BYTE *, TPM_AUTH *, UINT32 *,
                         BYTE **);
    TSS_RESULT (*KillMaintenanceFeature)(TSS_HCONTEXT, TPM_AUTH *);
    TSS_RESULT (*LoadManuMaintPub)(TSS_HCONTEXT, TCPA_NONCE, UINT32, BYTE *, TCPA_DIGEST *);
    TSS_RESULT (*ReadManuMaintPub)(TSS_HCONTEXT, TCPA_NONCE, TCPA_DIGEST *);
#endif
#ifdef TSS_BUILD_DAA
    TSS_RESULT (*DaaJoin)(TSS_HCONTEXT, TPM_HANDLE, BYTE, UINT32, BYTE*, UINT32, BYTE*,
                  TPM_AUTH*, UINT32*, BYTE**);
    TSS_RESULT (*DaaSign)(TSS_HCONTEXT, TPM_HANDLE, BYTE, UINT32, BYTE*, UINT32, BYTE*,
                  TPM_AUTH*, UINT32*, BYTE**);
#endif
#ifdef TSS_BUILD_COUNTER
    TSS_RESULT (*ReadCounter)(TSS_HCONTEXT, TSS_COUNTER_ID, TPM_COUNTER_VALUE*);
    TSS_RESULT (*CreateCounter)(TSS_HCONTEXT, UINT32, BYTE*, TPM_ENCAUTH, TPM_AUTH*,
                    TSS_COUNTER_ID*, TPM_COUNTER_VALUE*);
    TSS_RESULT (*IncrementCounter)(TSS_HCONTEXT, TSS_COUNTER_ID, TPM_AUTH*, TPM_COUNTER_VALUE*);
    TSS_RESULT (*ReleaseCounter)(TSS_HCONTEXT, TSS_COUNTER_ID, TPM_AUTH*);
    TSS_RESULT (*ReleaseCounterOwner)(TSS_HCONTEXT, TSS_COUNTER_ID, TPM_AUTH*);
#endif
#ifdef TSS_BUILD_TICK
    TSS_RESULT (*ReadCurrentTicks)(TSS_HCONTEXT, UINT32*, BYTE**);
    TSS_RESULT (*TickStampBlob)(TSS_HCONTEXT, TCS_KEY_HANDLE, TPM_NONCE*, TPM_DIGEST*,
                    TPM_AUTH*, UINT32*, BYTE**,UINT32*, BYTE**);
#endif
#ifdef TSS_BUILD_NV
    TSS_RESULT (*NV_DefineOrReleaseSpace)(TSS_HCONTEXT, UINT32, BYTE*, TCPA_ENCAUTH, TPM_AUTH*);
    TSS_RESULT (*NV_WriteValue)(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32, BYTE*, TPM_AUTH*);
    TSS_RESULT (*NV_WriteValueAuth)(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32, BYTE*,
                    TPM_AUTH*);
    TSS_RESULT (*NV_ReadValue)(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32*, TPM_AUTH*, BYTE**);
    TSS_RESULT (*NV_ReadValueAuth)(TSS_HCONTEXT, TSS_NV_INDEX, UINT32, UINT32*, TPM_AUTH*,
                       BYTE**);
#endif
#ifdef TSS_BUILD_AUDIT
    TSS_RESULT (*SetOrdinalAuditStatus)(TSS_HCONTEXT, TPM_AUTH *, UINT32, TSS_BOOL);
    TSS_RESULT (*GetAuditDigest)(TSS_HCONTEXT, UINT32, TPM_DIGEST *, UINT32 *, BYTE **,
                     TSS_BOOL *, UINT32 *, UINT32 **);
    TSS_RESULT (*GetAuditDigestSigned)(TSS_HCONTEXT, TCS_KEY_HANDLE, TSS_BOOL, TPM_NONCE *,
                       TPM_AUTH *, UINT32 *, BYTE **, TPM_DIGEST *,
                       TPM_DIGEST *, UINT32 *, BYTE **);
#endif
#ifdef TSS_BUILD_TSS12
    TSS_RESULT (*SetOperatorAuth)(TSS_HCONTEXT, TPM_SECRET *);
    TSS_RESULT (*FlushSpecific)(TSS_HCONTEXT, TCS_HANDLE, TPM_RESOURCE_TYPE);
#endif
#ifdef TSS_BUILD_DELEGATION
    TSS_RESULT (*Delegate_Manage)(TSS_HCONTEXT, TPM_FAMILY_ID, TPM_FAMILY_OPERATION, UINT32,
                      BYTE *, TPM_AUTH *, UINT32 *, BYTE **);
    TSS_RESULT (*Delegate_CreateKeyDelegation)(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, BYTE *,
                           TPM_ENCAUTH *, TPM_AUTH *, UINT32 *, BYTE **);
    TSS_RESULT (*Delegate_CreateOwnerDelegation)(TSS_HCONTEXT, TSS_BOOL, UINT32, BYTE *,
                             TPM_ENCAUTH *, TPM_AUTH *, UINT32 *, BYTE **);
    TSS_RESULT (*Delegate_LoadOwnerDelegation)(TSS_HCONTEXT, TPM_DELEGATE_INDEX, UINT32, BYTE *,
                           TPM_AUTH *);
    TSS_RESULT (*Delegate_ReadTable)(TSS_HCONTEXT, UINT32 *, BYTE **, UINT32 *, BYTE **);
    TSS_RESULT (*Delegate_UpdateVerificationCount)(TSS_HCONTEXT, UINT32, BYTE *, TPM_AUTH *,
                               UINT32 *, BYTE **);
    TSS_RESULT (*Delegate_VerifyDelegation)(TSS_HCONTEXT, UINT32, BYTE *);
    TSS_RESULT (*DSAP)(TSS_HCONTEXT, TPM_ENTITY_TYPE, TCS_KEY_HANDLE, TPM_NONCE *, UINT32,
               BYTE *, TCS_AUTHHANDLE *, TPM_NONCE *, TPM_NONCE *);
#endif
    TSS_RESULT (*FieldUpgrade)(TSS_HCONTEXT, UINT32, BYTE *, UINT32 *, BYTE **, TPM_AUTH *);
    TSS_RESULT (*SetRedirection)(TSS_HCONTEXT, TCS_KEY_HANDLE, UINT32, UINT32, TPM_AUTH *);

#ifndef USES_TCG_STD
    TSM_RESULT (*APCreate)(
        TSM_HCONTEXT            tspContext,         // in
        TCM_ENTITY_TYPE         entityType,         // in
        UINT32                  entityValue,        // in
        TCM_NONCE               callerNonce,        // in
        TCM_AUTHDATA*           pAuth,              // in, out
        TCS_AUTHHANDLE*         authHandle,         // out
        TCM_NONCE*              TcmNonce,           // out
        UINT32 *                AntiReplaySeq       // out
    );
    TSM_RESULT (*APTerminate)(
        TCS_CONTEXT_HANDLE      hContext,           // in
        TCS_AUTHHANDLE          authHandle,         // in
        TCM_AUTH*               pAuth               // in
    );

    TSM_RESULT (*SMS4Encrypt)(
        TCS_CONTEXT_HANDLE      hContext,           // in
        TCS_KEY_HANDLE          enckeyHandle,       // in
        BYTE*                   IV,                 // in
        UINT32                  inDataSize,         // in
        BYTE*                   inData,             // in
        TCM_AUTH*               pEncAuth,           // in, out
        UINT32*                 outDataSize,        // out
        BYTE**                  outData             // out
    );

    TSM_RESULT (*SMS4Decrypt)(
        TCS_CONTEXT_HANDLE      hContext,           // in
        TCS_KEY_HANDLE          DnckeyHandle,       // in
        BYTE*                   IV,                 // in
        UINT32                  inDataSize,         // in
        BYTE*                   inData,             // in
        TCM_AUTH*               pEncAuth,           // in, out
        UINT32*                 outDataSize,        // out
        BYTE**                  outData             // out
    );

    TSM_RESULT (*MakeIdentity)(     
        TCS_CONTEXT_HANDLE      hContext,                       // in
        TCM_ENCAUTH             identityAuth,                   // in
        TCM_CHOSENID_HASH       IDLabel_PrivCAHash,             // in
        UINT32                  idIdentityKeyInfoSize,          // in
        BYTE*                   idIdentityKeyInfo,              // in
        TCM_AUTH*               pSmkAuth,                       // in, out
        TCM_AUTH*               pOwnerAuth,                     // in, out
        UINT32*                 idIdentityKeySize,              // out
        BYTE**                  idIdentityKey,                  // out
        UINT32*                 pcIdentityBindingSize,          // out
        BYTE**                  prgbIdentityBinding,            // out
        UINT32*                 pcEndorsementCredentialSize,    // out
        BYTE**                  prgbEndorsementCredential,      // out
        UINT32*                 pcPlatformCredentialSize,       // out
        BYTE**                  prgbPlatformCredential,         // out
        UINT32*                 pcConformanceCredentialSize,    // out
        BYTE**                  prgbConformanceCredential       // out
    );

    TSM_RESULT (*CollatePekRequest)(
        TCS_CONTEXT_HANDLE      hContext,                       // in
        TCM_CHOSENID_HASH       IDLabel_PrivCAHash,             // in
        UINT32*                 pcEndorsementCredentialSize,    // out
        BYTE**                  prgbEndorsementCredential       // out
    );

    TSM_RESULT (*ActivatePEK)(
        TCS_CONTEXT_HANDLE    hContext,             // in
        TCM_ENCAUTH           KeyUsageAuth,         // in
        TCM_ENCAUTH           KeyMigrationAuth,     // in
        UINT32                PEKKeyInfoSize,       // in
        BYTE*                 PEKKeyInfo,           // in
        UINT32                PEKDataSize,          // in
        BYTE*                 PEKData,              // in
        UINT32*               EncSymKeySize,        // in
        BYTE**                EncSymKey,            // in
        TCM_AUTH*             pSmkAuth,             // in, out
        TCM_AUTH*             pOwnerAuth,           // in, out
        UINT32*               PekKeySize,           // out
        BYTE**                PekKey                // out
    );
    TSM_RESULT (*ActivatePEKCert)( 
        TCS_CONTEXT_HANDLE    hContext,             // in
        UINT32                blobSize,             // in
        BYTE*                 blob,                 // in
        TCM_AUTH*             ownerAuth,            // in, out
        UINT32*               SymmetricKeySize,     // out
        BYTE**                SymmetricKey          // out
    );

#endif
};

extern struct tcs_api_table tcs_normal_api;
extern struct tcs_api_table tcs_transport_api;

#endif
