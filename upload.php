<?php
$upload_dir = 'uploads';
exec("rm -rf uploads/*");
if (!empty($_FILES)) 
{ 
     $tempFile = $_FILES['file']['tmp_name'];//this is temporary server location
     
     // using DIRECTORY_SEPARATOR constant is a good practice, it makes your code portable.
     $uploadPath = dirname( __FILE__ ) . DIRECTORY_SEPARATOR . $upload_dir . DIRECTORY_SEPARATOR;
     
     // Adding timestamp with image's name so that files with same name can be uploaded easily.
     $mainFile = $uploadPath.time().'-'. $_FILES['file']['name'];

    $format = substr($_FILES['file']['name'],-4);
    $newFileName = "FOTO".$format;
    $targetFile =  $uploadPath.$newFileName;  // Create the absolute path of the uploaded file destination.

     
     move_uploaded_file($tempFile,$targetFile );

	
	$output = shell_exec('./dcraw -4 -D uploads/FOTO.*');

	$output = shell_exec('./pgmToMatrix/histogram uploads/FOTO.pgm out');
}
?>
