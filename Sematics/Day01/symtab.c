/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // TODO
  Type* newType = (Type*) malloc(sizeof(Type));
  newType->typeClass = type->typeClass;
  if (type->typeClass == TP_ARRAY) {
    newType->arraySize = type->arraySize;
    newType->elementType = duplicateType(type->elementType);
  }
  return newType;
}

int compareType(Type* type1, Type* type2) {
  // TODO
  if(type1->typeClass == type2->typeClass){
    if(type1->typeClass == TP_ARRAY){
      if(type1->arraySize == type2->arraySize || compareType(type1->elementType, type2->elementType) == 0){
        return 0;
      }
      else{
        return 1;
      }
    }
  }
  return 0; //typeclass1!=2
}

void freeType(Type* type) {
  // TODO
  switch (type->typeClass) {
    case TP_INT:
    case TP_CHAR:
      free(type);
      break;
    case TP_ARRAY:
      freeType(type->elementType);
      freeType(type);
      break;
  }
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
  ConstantValue *newIntConstantValue = (ConstantValue*)malloc(sizeof(ConstantValue));
  newIntConstantValue->type = TP_INT;
  newIntConstantValue->intValue = i;

  return newIntConstantValue;
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
  ConstantValue *newCharConstantValue = (ConstantValue*)malloc(sizeof(ConstantValue));

  newCharConstantValue->type = TP_CHAR;
  newCharConstantValue->charValue = ch;
  return newCharConstantValue;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
  ConstantValue *newConstantValue = (ConstantValue*)malloc(sizeof(ConstantValue));
  newConstantValue->type = v->type;
  if(v->type == TP_CHAR){
    newConstantValue->type = TP_CHAR;
  }
  else if(v->type == TP_INT){
    newConstantValue->type = TP_INT;
  }
  return newConstantValue;
}

/******************* Object utilities ******************************/

Scope* CreateScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* CreateProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = CreateScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* CreateConstantObject(char *name) {
  // TODO
  Object *newConstantObject = (Object*)malloc(sizeof(Object));
  
  strcpy(newConstantObject->name,name);
  newConstantObject->kind = OBJ_CONSTANT;
  newConstantObject->constAttrs = (ConstantAttributes*)malloc(sizeof(ConstantAttributes));
  newConstantObject->constAttrs->value = (ConstantValue*)malloc(sizeof(ConstantValue));

  return newConstantObject;
}

Object* CreateTypeObject(char *name) {
  // TODO
  Object *newTypeObject = (Object*)malloc(sizeof(Object));

  strcpy(newTypeObject->name, name);
  newTypeObject->kind = OBJ_TYPE;
  newTypeObject->typeAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
  newTypeObject->typeAttrs->actualType = (Type*)malloc(sizeof(Type));

  return newTypeObject;
}

Object* CreateVariableObject(char *name) {
  // TODO
  Object *newVariableObject = (Object*)malloc(sizeof(Object));
  strcpy(newVariableObject->name,name);
  newVariableObject->kind = OBJ_VARIABLE;
  newVariableObject->varAttrs = (VariableAttributes*)malloc(sizeof(VariableAttributes));
  newVariableObject->varAttrs->scope = symtab->currentScope;
  newVariableObject->varAttrs->type = NULL;

  return newVariableObject;
}

Object* CreateFunctionObject(char *name) {
  // TODO
  Object *newFunctionObject = (Object*)malloc(sizeof(Object));
  strcpy(newFunctionObject->name,name);
  newFunctionObject->kind = OBJ_FUNCTION;
  newFunctionObject->funcAttrs = (FunctionAttributes*)malloc(sizeof(FunctionAttributes));
  newFunctionObject->funcAttrs->scope = CreateScope(newFunctionObject,symtab->currentScope);
  newFunctionObject->funcAttrs->paramList = (ObjectNode*)malloc(sizeof(ObjectNode));
  newFunctionObject->funcAttrs->returnType = (Type*)malloc(sizeof(Type));

  return newFunctionObject;
}

Object* CreateProcedureObject(char *name) {
  // TODO
  Object *newProcedureObject = (Object*)malloc(sizeof(newProcedureObject));

  strcpy(newProcedureObject->name, name);
  newProcedureObject->kind = OBJ_PROCEDURE;
  newProcedureObject->procAttrs = (ProcedureAttributes*)malloc(sizeof(ProcedureAttributes));
  newProcedureObject->procAttrs->scope = CreateScope(newProcedureObject,symtab->currentScope);
  newProcedureObject->procAttrs->paramList = (ObjectNode*)malloc(sizeof(ObjectNode));

  return newProcedureObject;
}

Object* CreateParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object* newParameterObject = (Object*)malloc(sizeof(Object));
  strcpy(newParameterObject->name, name);
  newParameterObject->kind = OBJ_PARAMETER;
  newParameterObject->paramAttrs = (ParameterAttributes*)malloc(sizeof(ParameterAttributes));
  newParameterObject->paramAttrs->kind = kind;
  newParameterObject->paramAttrs->type = (Type*)malloc(sizeof(Type));
  newParameterObject->paramAttrs->function = owner;

  return newParameterObject;
}

void freeObject(Object* obj) {
  // TODO
  switch(obj->kind){
    case OBJ_CONSTANT:
      free(obj->constAttrs->value);
      free(obj->constAttrs);
      break;
    case OBJ_FUNCTION:
      freeReferenceList(obj->funcAttrs->paramList);
      freeType(obj->funcAttrs->returnType);
      freeScope(obj->funcAttrs->scope);
      free(obj->funcAttrs);
      break;
    case OBJ_PARAMETER:
      freeType(obj->paramAttrs->type);
      free(obj->paramAttrs->function);
      free(obj->paramAttrs);
      break;
    case OBJ_PROCEDURE:
      freeReferenceList(obj->procAttrs->paramList);
      freeScope(obj->procAttrs->scope);
      free(obj->procAttrs);
      break;
    case OBJ_PROGRAM:
      freeScope(obj->progAttrs->scope);
      free(obj->progAttrs);
      break;
    case OBJ_TYPE:
      free(obj->typeAttrs->actualType);
      free(obj->typeAttrs);
      break;
    case OBJ_VARIABLE:
      freeScope(obj->varAttrs->scope);
      freeType(obj->varAttrs->type);
      free(obj->varAttrs);
      break;
  }
  free(obj);
}

void freeScope(Scope* scope) {
  // TODO
  freeObjectList(scope->objList);
  free(scope);
}

void freeObjectList(ObjectNode *objList) {
  // TODO
  if(objList != NULL){
    freeObject(objList->object);
    freeObjectList(objList->next);
    objList = NULL;
  }
}

void freeReferenceList(ObjectNode *objList) {
  // TODO
  ObjectNode *curr = objList;
  while(objList != NULL) {
    curr = objList;
    objList = objList->next;
    free(curr);
  }
}

void AddObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
  ObjectNode *curr = objList;
  while (curr != NULL){
    if(strcmp(curr->object->name, name) == 0){
      return curr->object;
    }
    curr = curr->next;
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = CreateFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEI");
  param = CreateParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  AddObject(&(obj->procAttrs->paramList),param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEC");
  param = CreateParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  AddObject(&(obj->procAttrs->paramList),param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITELN");
  AddObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
      case OBJ_FUNCTION:
        AddObject(&(owner->funcAttrs->paramList), obj);
        break;
      case OBJ_PROCEDURE:
        AddObject(&(owner->procAttrs->paramList), obj);
        break;
      default:
        break;
    }
  }
 
  AddObject(&(symtab->currentScope->objList), obj);
}


