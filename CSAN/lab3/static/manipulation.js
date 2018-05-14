$(document).ready(function(){


    input = document.getElementById('file-upload');
    input.addEventListener('change', onSelectFile, false);

    if (window.location.pathname === "/files/"){
        $('#back-button').addClass('disabled');
    }

    $('#user-table').on('click', '.clickable-row', function(event) {

        if($(this).hasClass('active')){
            $(this).removeClass('active');
        } else {
            $(this).addClass('active');
        }

    });

    function onSelectFile(){
        if (input.files[0] !== ''){
            document.uploadForm.path.value = window.location.pathname;
            document.uploadForm.submit();
        }
    }

    function getSelectedFiles() {
        let items = [];
        $('#user-table tbody tr.file-names').each( function(){
            //add item to array
            if ($(this).hasClass('active')){
                items.push( $(this).find("td").eq(1).text());
            }
        });
        return items;
    }

    function deleteFiles(path, files){

        var form = $('<form></form>');

        form.attr("method", "post");
        form.attr("action", "/delete");
        form.attr("enctype", "multipart/form-data");

        var field = $('<input />');
        var params = $('<input />');

        var str = JSON.stringify(files);

        field.attr("type", "hidden");
        field.attr("name", "fileNames");
        field.attr("value", str);

        form.append(field);

        params.attr("type", "hidden");
        params.attr("name", "path");
        params.attr("value", path);

        form.append(params);

        $(document.body).append(form);
        form.submit();

    }

    function copyFiles(path, files){

        var form = $('<form></form>');

        form.attr("method", "post");
        form.attr("action", "/copy");
        form.attr("enctype", "multipart/form-data");

        var field = $('<input />');
        var params = $('<input />');

        var str = JSON.stringify(files);

        field.attr("type", "hidden");
        field.attr("name", "fileNames");
        field.attr("value", str);

        form.append(field);

        params.attr("type", "hidden");
        params.attr("name", "path");
        params.attr("value", path);

        form.append(params);

        $(document.body).append(form);
        form.submit();

    }

    function pasteFiles(path){

        var form = $('<form></form>');

        form.attr("method", "post");
        form.attr("action", "/paste");
        form.attr("enctype", "multipart/form-data");

        var params = $('<input />');

        params.attr("type", "hidden");
        params.attr("name", "path");
        params.attr("value", path);

        form.append(params);

        $(document.body).append(form);
        form.submit();

    }

    function cutFiles(path, files){

        var form = $('<form></form>');

        form.attr("method", "post");
        form.attr("action", "/cut");
        form.attr("enctype", "multipart/form-data");

        var field = $('<input />');
        var params = $('<input />');

        var str = JSON.stringify(files);

        field.attr("type", "hidden");
        field.attr("name", "fileNames");
        field.attr("value", str);

        form.append(field);

        params.attr("type", "hidden");
        params.attr("name", "path");
        params.attr("value", path);

        form.append(params);

        $(document.body).append(form);
        form.submit();

    }

    $('#back-button').click(function () {
       let path = window.location.href.split('/');
       console.log(path);
       while (path[path.length - 1] === ""){
           path.pop();
       }
       path.pop();
       console.log(path);
       window.location.replace(path.join('/') + "/");
    });

    $('#upload-button').click(function () {
        console.log(window.location.pathname);
        input.click();

    });

    $('#block-button').click(function () {
        if ($(this).hasClass('disabled')){
            return;
        }
        urlArray = getSelectedURL();
        postUsers("/block", urlArray);
    });

    $('#delete-button').click(function () {
        if ($(this).hasClass('disabled')){
            return;
        }
        namesArray = getSelectedFiles();
        deleteFiles(window.location.pathname, namesArray);
    });

    $('#copy-button').click(function () {
        if ($(this).hasClass('disabled')){
            return;
        }
        namesArray = getSelectedFiles();
        copyFiles(window.location.pathname, namesArray);
    })

    $('#paste-button').click(function () {
        if ($(this).hasClass('disabled')){
            return;
        }
        pasteFiles(window.location.pathname);
    })

    $('#cut-button').click(function(){
        if ($(this).hasClass('disabled')){
            return;
        }
        namesArray = getSelectedFiles();
        cutFiles(window.location.pathname, namesArray);
    })


});

