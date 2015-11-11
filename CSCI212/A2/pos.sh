#!/bin/bash
# SHANNON ARCHER | SJA998
#functions
addTransaction() {
	
	read -p "Item Description: " desc

	read -p "Price: " price

	read -p "Quantity Purchased: " quantity

	read -p "Date Purchased (DDMMMYY): " purchaseDate

	echo "$desc:$price:$quantity:$purchaseDate" >> items.txt

	options
}
removeSalesTransaction() {
	#search file for data
	IFS=$'\n'
	set $(cat items.txt)

	PS3="Choosing a record will remove it from the data file: "
	select line in "$@" "Quit"
	do
		case $line in 
		"Quit") 
			options ;;
		*) 
			if [[ "$line" != "" ]]; then

				declare -i count=0
				while read -r line
				do
				  ((count++))
				  case "$count" in
				    "$REPLY") continue;;
				    * ) echo "$line";;
				  esac
				done < items.txt > tmpfile && mv tmpfile items.txt

				options
			fi ;;
		esac
	done
}
editSales() {
	#search file for data
	IFS=$'\n'
	set $(cat items.txt)

	PS3="Choosing a record to edit: "
	select line in "$@" "Quit"
	do
		case $line in 
		"Quit") 
			options ;;
		*) 
			if [[ "$line" != "" ]]; then

				read -p "Item Description [`echo "$line" | cut -d: -f1`]:" desc
				read -p "Price [`echo "$line" | cut -d: -f2`]:" price
				read -p "Quantity Purchased [`echo "$line" | cut -d: -f3`]:" quantity
				read -p "Date Purchased (DDMMMYY) [`echo "$line" | cut -d: -f4`]:" purchaseDate

				if [[ -z $desc ]]; then
					desc=`echo "$line" | cut -d: -f1`
				fi
		
				if [[ -z $price ]]; then
					price=`echo "$line" | cut -d: -f2`
				fi

				if [[ -z $quantity ]]; then
					quantity=`echo "$line" | cut -d: -f3`
				fi

				if [[ -z $purchaseDate ]]; then
					purchaseDate=`echo "$line" | cut -d: -f4`
				fi

				#change items.txt with new stuff
				replacement=$(echo "$desc:$price:$quantity:$purchaseDate")
				
				declare -i count=0
				while read -r line
				do
				  ((count++))
				  case "$count" in
				    "$REPLY") continue;;
				    * ) echo "$line";;
				  esac
				done < items.txt > tmpfile && mv tmpfile items.txt

				echo $replacement >> items.txt

				options
			fi ;;
		esac
	done
}
inquireSales() {
	printf "Inquire Sales Transaction (by Date DDMMMMYY): "
	read purchaseDate

	count=0
	i=0
	#search file for data
	while read line;
	do 
		count=`expr $count + 1`

		# break up line
		date=`echo "$line" | cut -d: -f4`

		if [[ $date = $purchaseDate ]]; then
			echo "Record No.: $count"
			echo "Item: `echo "$line" | cut -d: -f1`"
			echo "Price: `echo "$line" | cut -d: -f2`"
			echo "Quantity: `echo "$line" | cut -d: -f3`"
			echo "Date Purchased: `echo "$line" | cut -d: -f4`"
			echo ""
			i=`expr $i + 1`
		fi
		
		if [[ `wc -l items.txt` = "$count" ]]; then 
			echo "There are $i transactions on $purchaseDate"
		fi
	done < items.txt


	options
}
summary() {
	count=0
	i=0
	#search file for data
	echo "Date"$'\t'"Total Sales"
	awk -F: '{v[$NF]+=$2*$3}END{for(x in v)print x, "    $" v[x]}' items.txt
	printf "Grand Total $"	
	awk -F: '{v[$NF]+=$2*$3}END{sum=0; for(x in v) sum += v[x]; printf("%d\n", sum)}' items.txt

	options
}
quit() {
	exit
}

options() {
	PS3="Please choose one: "

	options=("Add new sales transaction" "Remove sales transaction" "Edit sales entries" "Inquire sales transaction" "Sales summary report" "Quit")

	select opt in "${options[@]}"
	do
		case $opt in 
			"Add new sales transaction")
				addTransaction
				;;
			"Remove sales transaction")
				removeSalesTransaction
				;;
			"Edit sales entries")
				editSales
				;;
			"Inquire sales transaction")
				inquireSales
				;;
			"Sales summary report")
				summary
				;;
			"Quit")
				quit
				;;
		esac
	done
}

# print options
options

