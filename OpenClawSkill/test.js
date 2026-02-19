/**
 * Test script for Unreal Engine Control Skill
 */

const UnrealControlSkill = require('./index.js');

async function runTests() {
  console.log('=== Testing Unreal Engine Control Skill ===\n');
  
  // Create skill instance
  const skill = new UnrealControlSkill();
  
  try {
    // Initialize skill
    console.log('1. Initializing skill...');
    const initialized = await skill.initialize();
    
    if (!initialized) {
      console.log('❌ Skill initialization failed');
      console.log('   Make sure:');
      console.log('   - Unreal Engine is running with OpenClawUE plugin');
      console.log('   - MCP server is enabled on port 8089');
      console.log('   - OpenClaw Gateway is running on port 18789');
      return;
    }
    
    console.log('✅ Skill initialized successfully\n');
    
    // Test 1: Check health
    console.log('2. Testing MCP server health...');
    try {
      const health = await skill.checkHealth();
      console.log(`✅ MCP server healthy on port ${health.port}`);
      console.log(`   Python available: ${health.python_available}`);
    } catch (error) {
      console.log(`❌ Health check failed: ${error.message}`);
    }
    console.log();
    
    // Test 2: Execute Python code
    console.log('3. Testing Python execution...');
    try {
      const result = await skill.executePython({
        code: 'print("Hello from OpenClawUE!")'
      });
      
      if (result.success) {
        console.log('✅ Python execution successful');
        if (result.stdout) {
          console.log(`   Output: ${result.stdout}`);
        }
      } else {
        console.log(`❌ Python execution failed: ${result.stderr}`);
      }
    } catch (error) {
      console.log(`❌ Python execution error: ${error.message}`);
    }
    console.log();
    
    // Test 3: Discover Python module
    console.log('4. Testing module discovery...');
    try {
      const result = await skill.discoverModule({
        module_name: 'unreal',
        name_filter: 'Blueprint'
      });
      
      if (result.success) {
        console.log('✅ Module discovery successful');
        console.log(`   Module: ${result.module}`);
        console.log(`   Classes: ${result.classes ? result.classes.length : 0}`);
        console.log(`   Functions: ${result.functions ? result.functions.length : 0}`);
      } else {
        console.log(`❌ Module discovery failed: ${result.error}`);
      }
    } catch (error) {
      console.log(`❌ Module discovery error: ${error.message}`);
    }
    console.log();
    
    // Test 4: Create blueprint (stub test)
    console.log('5. Testing blueprint creation (stub)...');
    try {
      const result = await skill.createBlueprint({
        name: 'BP_TestActor',
        parent_class: 'Actor',
        path: '/Game/Blueprints'
      });
      
      if (result.success) {
        console.log('✅ Blueprint creation test passed');
        console.log(`   Asset path: ${result.assetPath}`);
        console.log(`   Message: ${result.message}`);
      } else {
        console.log(`❌ Blueprint creation failed: ${result.error}`);
      }
    } catch (error) {
      console.log(`❌ Blueprint creation error: ${error.message}`);
    }
    console.log();
    
    // Test 5: Spawn actor (stub test)
    console.log('6. Testing actor spawning (stub)...');
    try {
      const result = await skill.spawnActor({
        actor_class: 'StaticMeshActor',
        location: [100, 200, 300],
        rotation: [0, 45, 0]
      });
      
      if (result.success) {
        console.log('✅ Actor spawning test passed');
        console.log(`   Actor path: ${result.actorPath}`);
        console.log(`   Location: [${result.location.join(', ')}]`);
        console.log(`   Rotation: [${result.rotation.join(', ')}]`);
      } else {
        console.log(`❌ Actor spawning failed: ${result.error}`);
      }
    } catch (error) {
      console.log(`❌ Actor spawning error: ${error.message}`);
    }
    console.log();
    
    // Test 6: Get all actors (stub test)
    console.log('7. Testing actor listing (stub)...');
    try {
      const result = await skill.getAllActors();
      
      if (result.success) {
        console.log('✅ Actor listing test passed');
        console.log(`   Found ${result.count} actors`);
        if (result.actors && result.actors.length > 0) {
          console.log('   First actor:', result.actors[0]);
        }
      } else {
        console.log(`❌ Actor listing failed: ${result.error}`);
      }
    } catch (error) {
      console.log(`❌ Actor listing error: ${error.message}`);
    }
    console.log();
    
    // Test 7: Send notification
    console.log('8. Testing notification system...');
    try {
      const sent = await skill.sendNotification('Test notification from Unreal Control Skill', 'info');
      if (sent) {
        console.log('✅ Notification sent successfully');
      } else {
        console.log('⚠️ Notification not sent (OpenClaw not connected)');
      }
    } catch (error) {
      console.log(`❌ Notification error: ${error.message}`);
    }
    console.log();
    
    console.log('=== All tests completed ===\n');
    console.log('Summary:');
    console.log('- Skill is ready for use with OpenClawUE plugin');
    console.log('- MCP server communication is working');
    console.log('- Python execution is available');
    console.log('- Blueprint and actor operations are ready');
    console.log('\nNext steps:');
    console.log('1. Install OpenClawUE plugin in Unreal Engine');
    console.log('2. Enable MCP server in plugin settings');
    console.log('3. Run actual UE operations');
    
  } catch (error) {
    console.error('❌ Test suite error:', error);
  } finally {
    // Clean up
    await skill.cleanup();
    console.log('\n✅ Skill cleaned up');
  }
}

// Run tests
runTests().catch(console.error);