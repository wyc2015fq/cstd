# java restful webservice 之MultivaluedMap 例子 - z69183787的专栏 - CSDN博客
2013年04月26日 17:06:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9614
server：
package  com.test.web.resources;
import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.MultivaluedMap;
import org.springframework.stereotype.Component;
/**
 * 测试
 * @author ann
 *
*/
@Path("/test")
@Component
publicclass Test {
    @POST
    @Path("/formTest")
    @Produces({MediaType.APPLICATION_XML,MediaType.APPLICATION_JSON})
    @Consumes("application/x-www-form-urlencoded")
public String formTest(MultivaluedMap<String, String>  formParams) {
        String result  ="";
        result ="name:"+ formParams.getFirst("name") +" ; nickname:"+formParams.getFirst("nickname");
return result;
    }
@POST
    @Path("/formTest2")
    @Produces({MediaType.APPLICATION_XML,MediaType.APPLICATION_JSON})
    @Consumes("application/x-www-form-urlencoded")
    public String formTest2(@QueryParam("uid")Long uid,MultivaluedMap<String, String>  formParams) {
        String result  = "";
        result ="uid"+ uid + " ; name:" + formParams.getFirst("name") + " ; nickname:"+formParams.getFirst("nickname");
        return result;
    }
}
client 测试例子：
package com.test.test;
import javax.ws.rs.core.MultivaluedMap;
import com.sun.jersey.api.client.Client;
import com.sun.jersey.api.client.WebResource;
import com.sun.jersey.api.client.config.ClientConfig;
import com.sun.jersey.api.client.config.DefaultClientConfig;
import com.sun.jersey.core.util.MultivaluedMapImpl;
publicclass Test {
privatestaticfinal  String baseUrl="http://localhost:8082/TestServer";
publicvoid testMultivaluedMap (){
         ClientConfig cc =new DefaultClientConfig();
         Client c = Client.create(cc);
         MultivaluedMap  form =new MultivaluedMapImpl();
         form.add("name","ann zhang");
         form.add("nickname","ann ");
         WebResource wr = c.resource(baseUrl +"/test/formTest");
         String response = wr.post(String.class, form);
         System.out.println("result:"+response);
    }
public void testMultivaluedMap2 (){
         ClientConfig cc = new DefaultClientConfig();
         Client c = Client.create(cc);
         MultivaluedMap  form = new MultivaluedMapImpl();
         form.add("name","ann zhang");
         form.add("nickname","ann ");
         WebResource wr = c.resource(baseUrl + "/test/formTest2");
         String response = wr.queryParam("uid", "10").post(String.class, form);
         System.out.println("result:"+response);
    }
publicstaticvoid main(String[] args){
        Test test =new Test();
        test.testMultivaluedMap();
test.testMultivaluedMap2();
    }
}
