const container = document.querySelector('#list');
var inputValue = document.querySelector('#item');
var items;
var inputDate = document.querySelector('#item-date');
const add = document.querySelector('#button-add');
const search = document.querySelector('#button-search');
const search_input = document.querySelector('#search-input');

if(window.localStorage.getItem("todos") == undefined || window.localStorage.getItem("todosDate") == undefined){
     var todos = [];
     var todosDate = [];
     window.localStorage.setItem("todos", JSON.stringify(todos));
     window.localStorage.setItem("todosDate", JSON.stringify(todosDate));
}

var todosEX = window.localStorage.getItem("todos");
var todos = JSON.parse(todosEX);
var todosDateEX = window.localStorage.getItem("todosDate");
var todosDate = JSON.parse(todosDateEX);

class item{
	constructor(name,date){
		this.createItem(name,date);
	}
    createItem(name,date){
    	var itemBox = document.createElement('div');
        itemBox.classList.add('item');

    	var input = document.createElement('input');
    	input.type = "text";
    	input.value = name;
      input.disabled = true;
    	input.classList.add('item_input');
      
      var date_input = document.createElement('input'); 
      date_input.type = "text";
    	date_input.value = date;
      date_input.disabled = true;
    	date_input.classList.add('item_date_input');

    	var edit = document.createElement('button');
    	edit.classList.add('edit');
    	edit.innerHTML = "EDIT";
    	edit.addEventListener('click', () => this.edit(input, date_input,name));

    	var remove = document.createElement('button');
    	remove.classList.add('remove');
    	remove.innerHTML = "REMOVE";
    	remove.addEventListener('click', () => this.remove(itemBox, name));

    	container.appendChild(itemBox);

        itemBox.appendChild(input);
        itemBox.appendChild(date_input);
        itemBox.appendChild(edit);
        itemBox.appendChild(remove);
    }

    edit(input, date_input, name){
      if(input.disabled == true){
        input.disabled = false;
        date_input.disabled = false;
      }
      else{
            let indexof = todos.indexOf(name);
            todos[indexof] = input.value;
            todosDate[indexof] = date_input.value;
            window.localStorage.setItem("todos", JSON.stringify(todos));
            window.localStorage.setItem("todosDate", JSON.stringify(todosDate));
            input.disabled = true;
            date_input.disabled = true;
      }
    }

    remove(itemBox, name){
        itemBox.parentNode.removeChild(itemBox);
        let index = todos.indexOf(name);
        todos.splice(index, 1);
        todosDate.splice(index,1);
        window.localStorage.setItem("todos", JSON.stringify(todos));
        window.localStorage.setItem("todosDate", JSON.stringify(todosDate));
    }
}

add.addEventListener('click', check);

search.addEventListener('click', searching);

function searching() {
      var childDivs = container.getElementsByTagName('div');
      var filter = search_input.value;
    if(filter == ""){
          for( i=0; i< childDivs.length; i++ )
          {
              var childDiv = childDivs[i];
              childDiv.style.display='block';
          }
    }
  else{
    for( i=0; i< childDivs.length; i++ )
    {
        childDiv = childDivs[i];
        var childName = childDiv.getElementsByTagName('input')[0].value;
        if(filter.localeCompare(childName)){
            childDiv.style.display = 'none';
        }
        else{
            childDiv.style.display = 'block';
        }
    }
  }
  search_input.value ="";
}

function check(){
	if(inputValue.value != "" && inputValue.value.length > 2 && inputValue.value.length < 256){
    var date1 = new Date();
    date1.setHours(0,0,0,0);
    if (inputDate.value && new Date(inputDate.value).getTime() >= date1.getTime()){
        new item(inputValue.value,inputDate.value);
        todos.push(inputValue.value);
        todosDate.push(inputDate.value);
        window.localStorage.setItem("todos", JSON.stringify(todos));
        window.localStorage.setItem("todosDate", JSON.stringify(todosDate));
		    inputValue.value = "";
        inputDate.value = "";
    }
    else{
      alert("Date must be at least today!");
    }
	}
  else{
    alert("Task's name must be between 3-255 characters!");
  }
}


for (var v = 0 ; v < todos.length ; v++){
    new item(todos[v],todosDate[v]);
}