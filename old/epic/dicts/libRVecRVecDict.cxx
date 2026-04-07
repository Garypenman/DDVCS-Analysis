// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME libRVecRVecDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "Types.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR_Dictionary();
   static void ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR_TClassManip(TClass*);
   static void *new_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR(void *p = nullptr);
   static void *newArray_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR(Long_t size, void *p);
   static void delete_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR(void *p);
   static void deleteArray_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR(void *p);
   static void destruct_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >*)
   {
      ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >));
      static ::ROOT::TGenericClassInfo 
         instance("ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >", -2, "ROOT/RVec.hxx", 1529,
                  typeid(ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >) );
      instance.SetNew(&new_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR);
      instance.SetNewArray(&newArray_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR);
      instance.SetDelete(&delete_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR);
      instance.SetDeleteArray(&deleteArray_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR);
      instance.SetDestructor(&destruct_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> > >()));
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >*)
   {
      return GenerateInitInstanceLocal(static_cast<ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >*>(nullptr))->GetClass();
      ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> > : new ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >;
   }
   static void *newArray_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >[nElements] : new ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR(void *p) {
      delete (static_cast<ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >*>(p));
   }
   static void deleteArray_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR(void *p) {
      delete [] (static_cast<ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >*>(p));
   }
   static void destruct_ROOTcLcLVecOpscLcLRVeclEROOTcLcLVecOpscLcLRVeclEfloatgRsPgR(void *p) {
      typedef ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> > current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >

namespace {
  void TriggerDictionaryInitialization_libRVecRVecDict_Impl() {
    static const char* headers[] = {
"Types.h",
nullptr
    };
    static const char* includePaths[] = {
"/opt/software/linux-debian13-x86_64_v2/gcc-14.2.0/root-6.32.14-a5n6bmxxosdr57uxmozelkbp6m75vvkq/include/root",
"/opt/software/linux-debian13-x86_64_v2/gcc-14.2.0/root-6.32.14-a5n6bmxxosdr57uxmozelkbp6m75vvkq/include/root",
"/home/garyp/eic/DDVCS-Analysis/epic/dicts/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libRVecRVecDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace ROOT{namespace VecOps{template <typename T> class __attribute__((annotate(R"ATTRDUMP(__cling__ptrcheck(off))ATTRDUMP"))) __attribute__((annotate("$clingAutoload$ROOT/RVec.hxx")))  __attribute__((annotate("$clingAutoload$Types.h")))  RVec;
}}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRVecRVecDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Types.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ROOT::VecOps::RVec<ROOT::VecOps::RVec<float> >", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRVecRVecDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRVecRVecDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRVecRVecDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRVecRVecDict() {
  TriggerDictionaryInitialization_libRVecRVecDict_Impl();
}
