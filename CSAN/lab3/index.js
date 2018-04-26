const express = require('express');
const fs = require('fs');
const app = express();
const storageFolder = "./files/";
const fileUpload = require('express-fileupload');





app.use("/static", express.static(__dirname + '/static'));
app.use(fileUpload());

app.get('/', (req, res) => {
   res.redirect('/files/');
});

app.get('/files/*', (req, res) => {

    let path = '.' + req.originalUrl;
    if (path[path.length - 1] !== "/"){
        path += "/";
    }
    fs.readdir(path, (err, files) => {
        var folders = []; var trueFiles = [];
        files.forEach((file) => {
           let stats = fs.statSync(path + file);
           if (stats.isFile()){
               let data = { name: file, size: stats.size };
               trueFiles.push(data);
           } else if (stats.isDirectory()) {
               let data = { name: file, size: "" };
               folders.push(data);
           }
        });
        res.render('storage.ejs', {files: trueFiles, folders: folders});
    });
});

app.post('/files/*',  (req, res) => {

    let uploadedFile = req.files.recfile;
    let path = req.body.path.slice(1);
    if (path[path.length - 1] !== '/'){
        path += "/";
    }
    uploadedFile.mv(path + uploadedFile.name, (err) => {
        if (err){
            return res.status(500).send(err);
        }
        res.redirect('/' + path);
    });

});

app.post('/delete', (req, res) => {

    fileNames = JSON.parse(req.body.fileNames);

    let path = req.body.path.slice(1);
    if (path[path.length - 1] !== '/'){
        path += "/";
    }
    fileNames.forEach((name) => {
       fs.unlinkSync(path + name);
    });

    res.redirect('/' + path);
});

app.post('/copy', (req, res) => {


    fileNames = JSON.parse(req.body.fileNames);
    let path = req.body.path.slice(1);
    if (path[path.length - 1] !== '/'){
        path += "/";
    }

    // Clear copy directory

    fs.readdir("./copy", (err, files) => {
        files.forEach((file) => {
            fs.unlinkSync(path + file);
        })
    });

    console.log(fileNames);

    fileNames.forEach((name) => {

    })

});


app.listen(3000, () => console.log('Server starts listening...'));