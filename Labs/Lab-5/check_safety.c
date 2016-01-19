bool check_safety(int n_customer, int request[])
{
	bool finish[NUM_CUSTOMERS];
	int work[NUM_RESOURCES];
	//Initial allocation of the work
	for (int i = 0; i < NUM_RESOURCES; i++){
		work[i] = request[i];
	}

	//Initial allocation of which customers are safe
	for(int i = 0; i < NUM_CUSTOMERS; i++){
		finish[i] = false;
	}

	/*For each customer, go through all of the resources that customer needs. If the customer hasn't already been set to safe,
	and the customer's need's are lower or equal to the amount of work that needs to be done, then set the customer to safe.
	*/
	for (int j = 0; j < NUM_CUSTOMERS; j++){
		bool f = false; 
		if(finish[j] == false){
			for (int i = 0; i < NUM_RESOURCES; i++){
				if(need[j][i] > work[i]){
					f = true;	
				}
				if(f == false){
					for(int x = 0; x < NUM_RESOURCES; x++){
						work[x] = work[x] + allocation[j][x];
						finish[j] = true;
					}
					
				}
			}
		}
	}
	//Loop to check state of each customer. If the state of each customer is safe, then the system is safe from deadlock.
	bool state = true;
	for(int i = 0; i < NUM_CUSTOMERS; i++){
		if(finish[i] == false){
			state = false;
		}
	}
	//Return the safety of the state. True is a safe state, false is a non-safe state.
	return state;
}