/*  alloc.c
 *  some code taken from sim.c
 *  Source code for a simulator of the ILOC subset defined in
 *  "Engineering a Compiler" by Cooper and Torczon
 *  written by Todd Waterman
 *  11/30/00 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "instruction.h"
#include "machine.h"
#include "alloc.h"


// Declaration
int* reg_array = 0;
int* reg_next_use= 0;
int inst_tracker=0;
Instruction* last_ins;
int last_inst_tracker;

void push_uses(Instruction* inst, int reg) {
	if (inst->uses==NULL) {
		Operand* newregptr= (Operand*)malloc(sizeof(Operand));
		newregptr->value=reg;
		newregptr->next=NULL;
		inst->uses=newregptr;
	} else {
		Operand* newregptr= (Operand*)malloc(sizeof(Operand));
		newregptr->value=inst->uses->value;
		newregptr->next=inst->uses->next;
		inst->uses->next=newregptr;
		inst->uses->value=reg;
	}
}

void push_live_list(Instruction* inst, int reg) {
	if (inst->live_list==NULL) {
		Operand* newregptr= (Operand*)malloc(sizeof(Operand));
		newregptr->value=reg;
		newregptr->next=NULL;
		inst->live_list=newregptr;
	} else {
		Operand* newregptr= (Operand*)malloc(sizeof(Operand));
		newregptr->value=inst->live_list->value;
		newregptr->next=inst->live_list->next;
		inst->live_list->next=newregptr;
		inst->live_list->value=reg;
	}
}

void push_live_list_check(Instruction* inst, int reg) {
	//first search to see if reg is already present
	if (inst->live_list==NULL) { // add reg since it is not present
		Operand* newregptr= (Operand*)malloc(sizeof(Operand));
		newregptr->value=reg;
		newregptr->next=NULL;
		inst->live_list=newregptr;
		return;
	}
	//search first item
	if (inst->live_list->value==reg) {
		return;
	}
	//now search rest of list
	Operand* current=inst->live_list->next;
	Operand* prev=inst->live_list;
	while (current!=NULL && prev!=NULL) {
		if (reg==current->value) {
			return;
		}
		prev=current;
		current=current->next;		
	}
	//add reg since reg is not present
	Operand* newregptr= (Operand*)malloc(sizeof(Operand));
	newregptr->value=inst->live_list->value;
	newregptr->next=inst->live_list->next;
	inst->live_list->next=newregptr;
	inst->live_list->value=reg;
}


void remove_live_list(Instruction* inst, int reg) {
	if (inst->live_list==NULL) {
		return;
	}
	if (inst->live_list->value==reg) { //first operand is the one to remove
		Operand* temp=inst->live_list;
		inst->live_list=inst->live_list->next;
		temp->next=NULL; //not necessary
		free(temp);
		return;
	}
	Operand* current=inst->live_list->next;
	Operand* prev=inst->live_list;
	while (current!=NULL && prev!=NULL) {
		if (reg==current->value) {
			Operand* temp= current;
			prev->next=current->next;
			temp->next=NULL; //not necessary
			free(temp);
			return;
		}
		prev=current;
		current=current->next;		
	} //the register to remove wasn't even there
	return;
}

//Main
int main(int argc, char* argv[])
{
    int mem_size = 0;
    int reg_size = 0;
    int machine_initialized = 0;
    Instruction* code;

    /* Set default stall mode - branches and memory AND registers */
    set_stall_mode(3);
	reg_size = (int) strtol(argv[1],(char**)NULL,10);
	if (reg_size==0 || reg_size<0) { //that means the string input was not an integer
			fprintf(stderr,"Invalid argument for k. Must be an positive integer.\n");
	}
	//dynamically allocate register array
	if (reg_array != 0) {
   		reg_array = (int*) realloc(reg_array, reg_size * sizeof(int));
		reg_next_use= (int*) realloc(reg_array, reg_size * sizeof(int));
	} else {
    		reg_array = (int*) malloc(reg_size * sizeof(int));
    		reg_next_use = (int*) malloc(reg_size * sizeof(int));
	}
	mem_size = 1029;

    if (!machine_initialized)
	initialize_machine(reg_size,mem_size);

    code = parse();

    if (!code)
    {
	fprintf(stderr,"Error reading input file, simulator not run.\n");
	return 1;
    }

    simulate(code);
	inst_tracker=inst_tracker-1; //last increment was one too many
//	//print the array of registers with their priorities (for sorting later)
//	printf("reg_array: \n");
//	for(int i = 0; i < reg_size; i++) {
//    		printf("r%d: %d\n", i, reg_array[i]);
//	} 

	//now we have each register and its priority.
	//must sort by priority
	//must walk through code backwards to calculate LIVE list
	//must walk through code again forwards to replace registers appropriately,
	// adding loads/stores
	// and putting the new instructions in output file
	
	//Let's do a backward pass and construct LIVE list for each instruction
	Instruction* ins=last_ins;
	while(ins->prev!=NULL) {
		//add this instruction's live list to previous instruction's live list
		Operand* liveptr=ins->live_list;
		while (liveptr!=NULL) {
			push_live_list(ins->prev, liveptr->value);
			liveptr=liveptr->next;
		}		
		//add this instruction's uses to live list of previous instruction
		Operand* use= ins->uses;
		while (use!=NULL) {
			//add use to previous instruction's live list
			push_live_list_check(ins->prev, use->value);
			use=use->next; //go to next use	
		}
		//remove this instruction's target from prev instruction's live list
		if (ins->target!=NULL) {
			//remove target from previous instruction's live list
			remove_live_list(ins->prev,ins->target->value); 	
		}
		ins=ins->prev;
		inst_tracker= inst_tracker-1;
	}
	//now we've constructed a LIVE list for the VRs for each instruction (pre-optimization)
//	//lets see the live list
//	Instruction* insptr=ins;
//	for(int i = 0; i < 5; i++) {
//    		printf("INST# %d", i);
//		Operand* listptr=insptr->live_list;
//		while (listptr!=NULL) {
//			printf(" L: %d", listptr->value);
//			listptr=listptr->next;
//		}
//		printf("\n");
//		insptr=insptr->next;
//	} 

	//now lets reconstruct some instructions
	while(inst_tracker<=last_inst_tracker) {
		int op=ins->operations->opcode;
		if (op==1) { //ADD 
			printf("add r%d, r%d => r%d\n", ins->uses->next->value, 
				ins->uses->value, ins->target->value);
		} else if (op==2) { //MULT
			printf("mult r%d, r%d => r%d\n", ins->uses->next->value, 
				ins->uses->value, ins->target->value);
		} else if (op==3) { //LSHIFT
			printf("lshift r%d, r%d => r%d\n", ins->uses->next->value, 
				ins->uses->value, ins->target->value);
		} else if (op==4) { //RSHIFT
			printf("rshift r%d, r%d => r%d\n", ins->uses->next->value, 
				ins->uses->value, ins->target->value);
		} else if (op==5) { //LOADI
			printf("loadI %d => r%d\n", 
				ins->operations->consts->value, ins->target->value); //consts problem
		} else if (op==6) { //LOAD
			printf("load r%d => r%d\n", 
				ins->uses->value, ins->target->value);
		}else if (op==7) { //STORE
			printf("store r%d => r%d\n", ins->uses->next->value,
				ins->uses->value);
		} else if (op==8) { //OUTPUT
			printf("output %d\n", ins->operations->consts->value); //consts problem
		} else {
			printf("*******\n");
		}
		inst_tracker+=1;
		ins=ins->next;
	}
	free(reg_array);
    return 0;
};

/* Print a usage message */
void print_help()
{
    printf("Usage: sim [options] < filename\n");
    printf("  Options:\n");
    printf("    -h                 display usage message\n");
    printf("    -r NUM             simulator has NUM available registers\n");
    printf("    -m NUM             simulator has NUM bytes of memory\n");
    printf("    -s NUM             simulator stalls for the following conditions:\n");
    printf("                         0:  nothing\n");
    printf("                         1:  branches\n");
    printf("                         2:  branches and memory interlocks\n");
    printf("                         3:  branches and both register and memory interlocks\n");
    printf("    -i NUM ... NUM     starting at the memory location specified by the first\n");
    printf("                         NUM put the remaining NUMs into memory as words.\n");
    printf("                         Must be the last option specified\n");
    printf("    -c NUM ... NUM     same as -i, but puts the NUMs into memory as bytes\n");
    printf("  filename should be a valid ILOC input file\n");

}

/* Set stall flags appropriately */
void set_stall_mode(int mode)
{
    stall_on_branches = 0;
    stall_on_memory = 0;
    stall_on_registers = 0;

    switch(mode)
    {
	case 3:
	  stall_on_registers = 1;
	case 2:
	  stall_on_memory = 1;
	case 1:
	  stall_on_branches = 1;
	case 0:
	  break;
	default:
	  fprintf(stderr,"Illegal safety mode specified.\n");
	  exit(1);
      }
}


/* Simulate the code and output results to standard out */
void simulate(Instruction* code)
{
    Change* list_of_effects = NULL; 
    Change* last_effect = NULL;
    Change* new_effects;
    int instruction_count = 0;
    int operation_count = 0;
    int cycle_count = 0;
	code->prev=NULL;
    while(code)
    {
	if (!((memory_stall(code,list_of_effects) && stall_on_memory) ||
	      (register_stall(code,list_of_effects) && stall_on_registers) ||
	      (branch_stall(list_of_effects) && stall_on_branches)))
	{
/* The first lab doesn't need to deal with machine constraints. 
 *	    if (!check_machine_constraints(code))
 *	    {
 *		fprintf(stderr,"Error: Machine constraints violated.\n");
 *		exit(1);
 *	    }
 */		
	    new_effects = execute_instruction(code,&operation_count);
	    instruction_count++;

	    if (!list_of_effects)
	    {
		list_of_effects = new_effects;
		last_effect = new_effects;
	    }
	    else
		last_effect->next = new_effects;

	    /* Move last effect to end */
	    if (last_effect)
		while(last_effect->next)
		    last_effect = last_effect->next;

	    /* Go to next instruction */
		//fprintf("Instr: %d\n", code->operations->opcode);

		//keep track of pointer to previous instruction
		if (code->next != NULL) {
			code->next->prev= code;
		} else {
			last_ins=code;
			last_inst_tracker=inst_tracker;
		}

	    code = code->next;
		inst_tracker+=1;
	}
	list_of_effects = execute_changes(list_of_effects,&last_effect,&code);
	cycle_count++;

    }

    while(list_of_effects)
    {
	list_of_effects = execute_changes(list_of_effects,&last_effect,&code);
	cycle_count++;
    }

}

/* Returns 1 if the instruction uses a register that is not ready */
int register_stall(Instruction* inst,Change* changes)
{
    Operation* current_op = inst->operations;

    while(current_op)
    {
	/* Check source registers for operation */
	if (!list_of_operands_ready(current_op->srcs,changes, inst))
	    return 1;

	/* Also check target registers on stores */
	if ((opcode_specs[current_op->opcode].target_is_source) &&
	    (!list_of_operands_ready(current_op->defs,changes, inst)))
	    return 1;

	current_op = current_op->next;
    }

    /* All registers are ready if this point is reached */
    return 0;
}

/* Returns 1 if all operands in the list are ready, and 
   return 0 if they are not */
int list_of_operands_ready(Operand* reg, Change* changes, Instruction* inst)
{
    while(reg)
    {
	/* Non zero values in register_ready indicate that
	   the register is not ready */
	if (!reg_ready(reg->value,changes, inst))
	    return 0;
	reg = reg->next;
    }
    
    /* All register are ready if this point is reached */
    return 1;
}

/* Returns 1 if the instruction uses a memory address that is not ready */
int memory_stall(Instruction* inst, Change* changes)
{
    int memory_location;
    Operation* current_op = inst->operations;

    while(current_op)
    {
	switch(current_op->opcode)
	{
	    case LOAD:
	    memory_location = get_register(current_op->srcs->value);
	    if (!word_ready(memory_location,changes))
		return 1;
	    break;

	    case OUTPUT:
	    memory_location = current_op->consts->value;
	    if (!mem_ready(memory_location,changes))
		return 1;
	    break;


	    default:
	    break;
	
	}

	current_op = current_op->next;
    }
    /* All memory locations are ready if this point is reached */
    return 0;
}

/* Returns 1 if the register does not depend on the list of effects */
int reg_ready(int reg, Change* effects, Instruction* inst)
{
    while(effects)
    {
	if (effects->type == REGISTER && effects->location == reg)
	    return 0;
	effects = effects->next;
    }
 //   if (inst->operations->opcode!=8) { //make sure operationis not "output"
	//live range computation
	//for this instruction, need to add reg to the list of uses
	//when passing backwards through instructions, will add uses to LIVE list for rule above
	//also, globally record this instruction # as the next use. will be used to break ties for spills
	push_uses(inst, reg);
	//increment frequency
	reg_array[reg] +=1;
	//printf("reg_array[%d]: %d\n", reg, reg_array[reg]);
  //  }
    return 1;
}

/* Returns 1 if the memory location does not depend on the list of effects */
int mem_ready(int location, Change* effects)
{
    while(effects)
    {
	if (effects->type == MEMORY && effects->location == location)
	    return 0;
	effects = effects->next;
    }
    return 1;
}

/* Returns 1 if the word of memory does not depend on the list of effects */
int word_ready(int location, Change* effects)
{
    while(effects)
    {
	if (effects->type == MEMORY && effects->location >= location
	    && effects->location <= (location+3))
	    return 0;
	effects = effects->next;
    }
    return 1;
}

/* Execute all operations in a single instruction */
Change* execute_instruction(Instruction* inst, int* op_count)
{
    Operation* current_op = inst->operations;
    Change* all_changes = NULL;
    Change* last_change;
    Change* new_changes;

    while(current_op)
    {
	(*op_count)++;
	new_changes = execute_operation(current_op, inst);
	
	if (!all_changes)
	{
	    all_changes = new_changes;
	    last_change = new_changes;
	}
	else
	    last_change->next = new_changes;

	/* Move last change to end */
	if (last_change)
	    while(last_change->next)
		last_change = last_change->next;
	
		
	current_op = current_op->next;
    }

    return(all_changes);


}

/* Execute a single operation */
Change* execute_operation(Operation* op, Instruction* inst)
{
    Change* effects;
    /* This is a big, ugly switch statement that deals with every operation */
    switch(op->opcode)
    {
	case NOP:
	  return NULL;
	  break;

	case ADD:
	  effects = onereg(op, inst);
	  effects->value = get_register(op->srcs->value) + 
	      get_register(op->srcs->next->value);
	  return effects;
	  break;

	case MULT:
	  effects = onereg(op, inst);
	  effects->value = get_register(op->srcs->value) * 
	      get_register(op->srcs->next->value);
	  return effects;
	  break;

	case LSHIFT:
	  effects = onereg(op, inst);
	  effects->value = get_register(op->srcs->value) << 
	      get_register(op->srcs->next->value);
	  return effects;
	  break;

	case RSHIFT:
	  effects = onereg(op, inst);
	  effects->value = get_register(op->srcs->value) >> 
	      get_register(op->srcs->next->value);
	  return effects;
	  break;

	case LOADI:
	  effects = onereg(op, inst);
		inst->operations->consts->value=op->consts->value;
	  effects->value = op->consts->value;
	  return effects;
	  break;

	case LOAD:
	  effects = onereg(op, inst);
	  effects->value = get_word(get_register(op->srcs->value));
	  return effects;
	  break;
	  
	case STORE:
	  effects = storeop(op);
          effects->value = get_register(op->srcs->value);
          effects->location = get_register(op->defs->value);
	  return effects;

	  break;
	  
	case OUTPUT:
	  effects = (Change*)malloc(sizeof(Change));
	  effects->type = DISPLAY;
	  effects->cycles_away = opcode_specs[OUTPUT].latency;
	  effects->next = NULL;
		inst->operations->consts->value=op->consts->value;
	  effects->value = get_word(op->consts->value);
	  return effects;
	  break;

	default:
	  fprintf(stderr,"Simulator Error: Invalid opcode encountered in execute_operation.");
	  return NULL;
	  break;
      }
}

/* onereg creates most of a change structure for the common case where 
   a single register is defined. */
Change* onereg(Operation* op, Instruction* inst)
{
	int reg=op->defs->value;
    Change* effect = (Change*)malloc(sizeof(Change));
    effect->type = REGISTER;
    effect->location = op->defs->value; 
	//live range computation
	//for this instruction, must add reg to TARGET
	//when passing backwards thru instructions, will subtract TARGET from LIVE list for rule above
	//also, globally record this instruction as the next use. will be used to break ties for spills
	
	//add reg as target
	Operand* newregptr= (Operand*)malloc(sizeof(Operand));
	newregptr->value=reg;
	newregptr->next=NULL;
	inst->target=newregptr;

	//add to frequency
	reg_array[reg] +=1;
	//printf("reg_array[%d]: %d\n", reg, reg_array[reg]);
    effect->cycles_away = opcode_specs[op->opcode].latency;
    effect->next = NULL;
    return effect;
}

/* storeop creates most of a change structure for a store operation */
Change* storeop(Operation* op)
{
    Change* effect = (Change*)malloc(sizeof(Change));
    effect->type = MEMORY;
    effect->cycles_away = opcode_specs[op->opcode].latency;
    effect->next = NULL;
    return effect;
}




/* Returns 1 if there is an outstanding branch instruction */
int branch_stall(Change* changes)
{
    while(changes)
    {
	if (changes->type == BRANCH)
	    return 1;
	changes = changes->next;
    }
    return 0;
}

/* Reduces the cycles_away of all changes by one and executes any changes
   that have a cycles_away of 0 */
Change* execute_changes(Change* changes, Change** last_change_ptr, Instruction** code_ptr)
{
    Change* first_change = changes;
    Change* prev_change = NULL;

    /* Iterate through all changes */
    while(changes)
    {
	changes->cycles_away -= 1;
	
	if (changes->cycles_away == 0)
	{
	    /* Perform effect */
	    switch(changes->type)
	    {
		case REGISTER:
		  set_register(changes->location,changes->value);
		  break;
		case MEMORY:
		  set_memory(changes->location,changes->value);
		  break;
		case BRANCH:
		  *code_ptr = changes->target;
		  break;
		case DISPLAY:
		  fprintf(stdout,"%d\n",changes->value);
		  break;
	      }

	    /* Delete change record */
	    if (prev_change)
	    {
		prev_change->next = changes->next;
		free(changes);
		changes = prev_change->next;
	    }
	    else
	    {
		first_change = changes->next;
		free(changes);
		changes = first_change;
	    }
	}
	else
	{
	    prev_change = changes;
	    changes = changes->next;
	}

    }

    *last_change_ptr = prev_change;
    return (first_change);
}

int check_machine_constraints(Instruction* inst)
{
    Operation* current_op = inst->operations;

    int memory_op_count = 0;
    int mult_op_count = 0;
    int op_count = 0;
    int output_op_count = 0;

    while(current_op)
    {
	op_count++;
	switch(current_op->opcode)
	{
	    case LOAD:
	    case STORE:
	    memory_op_count++;
	    break;

	    case MULT:
	    mult_op_count++;
	    break;

 	    case OUTPUT:
	    output_op_count++;
	    break;

	    default:
	    break;
	}

	current_op = current_op->next;
    }

    return (op_count <= 2 && mult_op_count <= 1 && memory_op_count <= 1);
}



