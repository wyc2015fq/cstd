# Angular HttpClient upload file with FormData - =朝晖= - 博客园
# [Angular HttpClient upload file with FormData](https://www.cnblogs.com/dhcn/p/7802667.html)
     从sof上找到一个example：https://stackoverflow.com/questions/46206643/asp-net-core-2-0-and-angular-4-3-file-upload-with-progress，不但上传文件，而且支持多文件：
      模板代码：
```
<input #file type="file" multiple (change)="upload(file.files)" />
<span *ngIf="uploadProgress > 0 && uploadProgress < 100">
    {{uploadProgress}}%
</span>
```
       组件代码：
```
import { Component } from '@angular/core';
import { HttpClient, HttpRequest, HttpEventType, HttpResponse } from '@angular/common/http'
@Component({
    selector: 'files',
    templateUrl: './files.component.html',
})
export class FilesComponent {
    public uploadProgress: number;
    constructor(private http: HttpClient) { }
    upload(files) {
        if (files.length === 0)
            return;
        const formData = new FormData();
        for (let file of files)
            formData.append(file.name, file);
        const req = new HttpRequest('POST', `api/files`, formData, {
            reportProgress: true,
        });
        this.http.request(req).subscribe(event => {
            if (event.type === HttpEventType.UploadProgress)
                this.uploadProgress = Math.round(100 * event.loaded / event.total);
            else if (event instanceof HttpResponse)
                console.log('Files uploaded!');
        });
    }
}
```
      单文件上传，改改就行。
      另外那个FormData接口的使用需要IE9兼容填充库：https://angular.cn/guide/browser-support#建议的填充库

